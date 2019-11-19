//
// Created by dengchong on 2019-09-25.
//

#include "GLSurfaceRender.h"

#define LOG_TAG "GLSurfaceRender"

GLSurfaceRender::GLSurfaceRender(int viewWidth, int viewHeight, const char *vertexSource,
                                 const char *fragmentSource)
        : viewWidth(viewWidth),
          viewHeight(viewHeight),
          vertexSource(vertexSource),
          fragmentSource(fragmentSource) {
}

void GLSurfaceRender::init() {
    programId = loadProgram(vertexSource, fragmentSource);
    if (!programId) {
        LOGE("Could not create Program");
        return;
    }
    vertexcoordAttrLoc = static_cast<GLuint>(glGetAttribLocation(programId, "vertexcoord"));
    checkGLError("glGetAttribLocation vertexcoord");
    texcoordAttrLoc = static_cast<GLuint>(glGetAttribLocation(programId, "texcoord"));
    checkGLError("glGetAttribLocation texcoord");
    textureUniformLoc = static_cast<GLuint>(glGetAttribLocation(programId, "texture"));
    checkGLError("glGetAttribLocation texture");
    isInitProgram = true;
}

GLSurfaceRender::~GLSurfaceRender() {
    if (programId) {
        glDeleteProgram(programId);
    }
}

void GLSurfaceRender::resetRenderSize(int left, int top, int width, int height) {
    this->left = left;
    this->top = top;
    this->viewWidth = width;
    this->viewHeight = height;
}

void GLSurfaceRender::renderToView(GLuint texId) {
    glViewport(left, top, viewWidth, viewHeight);
    if (!isInitProgram) {
        LOGE("renderToView error , isInitProgram = false");
        return;
    }

    glUseProgram(programId);
    //todo 使用 VAO
    glVertexAttribPointer(vertexcoordAttrLoc, 2, GL_FLOAT, GL_FALSE, 0, OUTPUT_VIEW_VERTEX_COORD);
    glEnableVertexAttribArray(vertexcoordAttrLoc);
    glVertexAttribPointer(texcoordAttrLoc, 2, GL_FLOAT, GL_FALSE, 0, OUTPUT_VIEW_TEXTURE_COORD);
    glEnableVertexAttribArray(texcoordAttrLoc);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texId);
    glUniform1i(textureUniformLoc, 0);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisableVertexAttribArray(texcoordAttrLoc);
    glDisableVertexAttribArray(vertexcoordAttrLoc);
}

void GLSurfaceRender::renderToView(GLuint texId, int viewWidth, int viewHeight) {
    glViewport(0, 0, viewWidth, viewHeight);
    if (!isInitProgram) {
        LOGE("renderToView error , isInitProgram = false");
        return;
    }

    glUseProgram(programId);
    //todo 使用 VAO
    glVertexAttribPointer(vertexcoordAttrLoc, 2, GL_FLOAT, GL_FALSE, 0, OUTPUT_VIEW_VERTEX_COORD);
    glEnableVertexAttribArray(vertexcoordAttrLoc);
    glVertexAttribPointer(texcoordAttrLoc, 2, GL_FLOAT, GL_FALSE, 0, OUTPUT_VIEW_TEXTURE_COORD);
    glEnableVertexAttribArray(texcoordAttrLoc);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texId);
    glUniform1i(textureUniformLoc, 0);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisableVertexAttribArray(texcoordAttrLoc);
    glDisableVertexAttribArray(vertexcoordAttrLoc);
}

void GLSurfaceRender::renderToViewWithAutoFit(GLuint texId, int viewWidth, int viewHeight,
                                              int texWidth, int texHeight) {
    glViewport(0, 0, viewWidth, viewHeight);
    if (!isInitProgram) {
        LOGE("renderToView error , isInitProgram = false");
        return;
    }

    float texAspectRatio = static_cast<float>(texHeight) / texWidth;
    float screenAspectRatio = static_cast<float>(viewHeight) / viewWidth;
    float xOffset = 0.0f;
    float yOffset = 0.0f;
    if (texAspectRatio > screenAspectRatio) {
        int fillWidth = texWidth * viewHeight / texHeight;
        xOffset = static_cast<float >(fillWidth - viewWidth) / (2 * fillWidth);
        LOGI("GLSurfaceRender::renderToViewWithAutoFit , fillWidth : %d, swidth : %d, texwidth : %d,sheight : %d, texheight : %d,  xOffset : %f",
             fillWidth, viewWidth, texWidth, viewHeight, texHeight, xOffset);
    } else {
        int fillHeight = texHeight * viewWidth / texWidth;
        yOffset = static_cast<float >(fillHeight - viewHeight) / (2 * fillHeight);
        LOGI("GLSurfaceRender::renderToViewWithAutoFit , fillHeight : %d, swidth : %d, texwidth : %d,sheight : %d, texheight : %d,  yOffset : %f",
             fillHeight, viewWidth, texWidth, viewHeight, texHeight, yOffset);
    }
    LOGI("GLSurfaceRender: xOffset : %f , yOffset : %f", xOffset, yOffset);
    const GLfloat fitTexcoord[] = {
            xOffset, yOffset,
            1.0f - xOffset, yOffset,
            xOffset, 1.0f - yOffset,
            1.0f - xOffset, 1.0f - yOffset
    };
    glUseProgram(programId);
    //todo 使用 VAO
    glVertexAttribPointer(vertexcoordAttrLoc, 2, GL_FLOAT, GL_FALSE, 0, OUTPUT_VIEW_VERTEX_COORD);
    glEnableVertexAttribArray(vertexcoordAttrLoc);
    glVertexAttribPointer(texcoordAttrLoc, 2, GL_FLOAT, GL_FALSE, 0, fitTexcoord);
    glEnableVertexAttribArray(texcoordAttrLoc);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texId);
    glUniform1i(textureUniformLoc, 0);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisableVertexAttribArray(texcoordAttrLoc);
    glDisableVertexAttribArray(vertexcoordAttrLoc);
}

void GLSurfaceRender::renderToViewWithAutoFill(GLuint texId, int viewWidth, int viewHeight,
                                               int texWidth, int texHeight) {
    glViewport(0, 0, viewWidth, viewHeight);
    if (!isInitProgram) {
        LOGE("renderToView error , isInitProgram = false");
        return;
    }

    float texAspectRatio = static_cast<float>(texHeight) / texWidth;
    float screenAspectRatio = static_cast<float>(viewHeight) / viewWidth;
    float xOffset = 0.0f;
    float yOffset = 0.0f;
    if (texAspectRatio > screenAspectRatio) {
        int fillHeight = texWidth * viewHeight / viewWidth;
        yOffset = static_cast<float>(texHeight - fillHeight) / (2 * texHeight);
//        LOGI("GLSurfaceRender::renderToViewWithAutoFill , fillHeight : %d, swidth : %d, texwidth : %d,sheight : %d, texheight : %d,  yOffset : %f",
//             fillHeight, viewWidth, texWidth, viewHeight, texHeight, yOffset);
    } else {
        int fillWidth = texHeight * viewWidth / viewHeight;
        xOffset = static_cast<float>(texWidth - fillWidth) / (2 * texWidth);
//        LOGI("GLSurfaceRender::renderToViewWithAutoFill , fillWidth : %d, swidth : %d, texwidth : %d,sheight : %d, texheight : %d,  xOffset : %f",
//             fillWidth, viewWidth, texWidth, viewHeight, texHeight, xOffset);
    }
    const GLfloat fitTexcoord[] = {
            xOffset, yOffset,
            1.0f - xOffset, yOffset,
            xOffset, 1.0f - yOffset,
            1.0f - xOffset, 1.0f - yOffset
    };
    glUseProgram(programId);
    //todo 使用 VAO
    glVertexAttribPointer(vertexcoordAttrLoc, 2, GL_FLOAT, GL_FALSE, 0, OUTPUT_VIEW_VERTEX_COORD);
    glEnableVertexAttribArray(vertexcoordAttrLoc);
    glVertexAttribPointer(texcoordAttrLoc, 2, GL_FLOAT, GL_FALSE, 0, fitTexcoord);
    glEnableVertexAttribArray(texcoordAttrLoc);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texId);
    //设置纹理四周为黑色
    float borderColor[] = {0.0f, 0.0f, 0.0f, 0.0f};
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER_OES);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER_OES);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR_OES, borderColor);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR_OES, borderColor);
    glUniform1i(textureUniformLoc, 0);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisableVertexAttribArray(texcoordAttrLoc);
    glDisableVertexAttribArray(vertexcoordAttrLoc);
}

void GLSurfaceRender::renderToTexture(GLuint inputTexId, GLuint outputTexId) {
    glViewport(left, top, viewWidth, viewHeight);
    if (!isInitProgram) {
        LOGE("renderToTexture error , isInitProgram = false");
        return;
    }

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, outputTexId, 0);
    checkGLError("renderToTexture , glFramebufferTexture2D");
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        LOGE("failed to make complete framebuffer object %x", status);
    }

    glUseProgram(programId);
    //todo 使用VAO
    glVertexAttribPointer(vertexcoordAttrLoc, 2, GL_FLOAT, GL_FALSE, 0, OUTPUT_VIEW_VERTEX_COORD);
    glEnableVertexAttribArray(vertexcoordAttrLoc);
    glVertexAttribPointer(texcoordAttrLoc, 2, GL_FLOAT, GL_FALSE, 0, OUTPUT_VIEW_TEXTURE_COORD);
    glEnableVertexAttribArray(texcoordAttrLoc);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, inputTexId);
    glUniform1i(textureUniformLoc, 0);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisableVertexAttribArray(texcoordAttrLoc);
    glDisableVertexAttribArray(vertexcoordAttrLoc);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);
}

void GLSurfaceRender::renderToVFlipTexture(GLuint inputTexId, GLuint outputTexId) {
    glViewport(left, top, viewWidth, viewHeight);
    if (!isInitProgram) {
        LOGE("renderToTexture error , isInitProgram = false");
        return;
    }

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, outputTexId, 0);
    checkGLError("renderToTexture , glFramebufferTexture2D");
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        LOGE("failed to make complete framebuffer object %x", status);
    }

    glUseProgram(programId);
    //todo 使用VAO
    glVertexAttribPointer(vertexcoordAttrLoc, 2, GL_FLOAT, GL_FALSE, 0, OUTPUT_VIEW_VERTEX_COORD);
    glEnableVertexAttribArray(vertexcoordAttrLoc);
    glVertexAttribPointer(texcoordAttrLoc, 2, GL_FLOAT, GL_FALSE, 0,
                          OUTPUT_VIEW_VFLIP_TEXTURE_COORD);
    glEnableVertexAttribArray(texcoordAttrLoc);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, inputTexId);
    glUniform1i(textureUniformLoc, 0);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisableVertexAttribArray(texcoordAttrLoc);
    glDisableVertexAttribArray(vertexcoordAttrLoc);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);
}

void GLSurfaceRender::renderToAutoFillTexture(GLuint inputTexId, int texWidth, int texHeight,
                                              GLuint outputTexId) {
    glViewport(left, top, viewWidth, viewHeight);
    if (!isInitProgram) {
        LOGE("renderToTexture error , isInitProgram = false");
        return;
    }

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, outputTexId, 0);
    checkGLError("renderToTexture , glFramebufferTexture2D");
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        LOGE("failed to make complete framebuffer object %x", status);
    }

    float texAspectRatio = static_cast<float>(texHeight) / texWidth;
    float screenAspectRatio = static_cast<float>(viewHeight) / viewWidth;
    float xOffset = 0.0f;
    float yOffset = 0.0f;
    if (texAspectRatio > screenAspectRatio) {
        int fillHeight = texWidth * viewHeight / viewWidth;
        yOffset = static_cast<float>(texHeight - fillHeight) / (2 * texHeight);
    } else {
        int fillWidth = texHeight * viewWidth / viewHeight;
        xOffset = static_cast<float>(texWidth - fillWidth) / (2 * texWidth);
    }
    const GLfloat fitTexcoord[] = {
            xOffset, yOffset,
            1.0f - xOffset, yOffset,
            xOffset, 1.0f - yOffset,
            1.0f - xOffset, 1.0f - yOffset
    };

    glUseProgram(programId);
    //todo 使用VAO
    glVertexAttribPointer(vertexcoordAttrLoc, 2, GL_FLOAT, GL_FALSE, 0, OUTPUT_VIEW_VERTEX_COORD);
    glEnableVertexAttribArray(vertexcoordAttrLoc);
    glVertexAttribPointer(texcoordAttrLoc, 2, GL_FLOAT, GL_FALSE, 0, fitTexcoord);
    glEnableVertexAttribArray(texcoordAttrLoc);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, inputTexId);
    //设置纹理四周为黑色
    float borderColor[] = {0.0f, 0.0f, 0.0f, 0.0f};
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER_OES);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER_OES);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR_OES, borderColor);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR_OES, borderColor);
    glUniform1i(textureUniformLoc, 0);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisableVertexAttribArray(texcoordAttrLoc);
    glDisableVertexAttribArray(vertexcoordAttrLoc);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);
}

void GLSurfaceRender::renderToAutoFitTexture(GLuint inputTexId, int texWidth, int texHeight,
                                             GLuint outputTexId) {
    glViewport(left, top, viewWidth, viewHeight);
    if (!isInitProgram) {
        LOGE("renderToTexture error , isInitProgram = false");
        return;
    }

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, outputTexId, 0);
    checkGLError("renderToTexture , glFramebufferTexture2D");
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        LOGE("failed to make complete framebuffer object %x", status);
    }

    float texAspectRatio = static_cast<float>(texHeight) / texWidth;
    float screenAspectRatio = static_cast<float>(viewHeight) / viewWidth;
    float xOffset = 0.0f;
    float yOffset = 0.0f;
    if (texAspectRatio > screenAspectRatio) {
        int fillWidth = texWidth * viewHeight / texHeight;
        xOffset = static_cast<float >(fillWidth - texWidth) / (2 * fillWidth);
    } else {
        int fillHeight = texHeight * viewWidth / texWidth;
        yOffset = static_cast<float >(fillHeight - viewHeight) / (2 * fillHeight);
    }
    LOGI("GLSurfaceRender: xOffset : %f , yOffset : %f", xOffset, yOffset);
    const GLfloat fitTexcoord[] = {
            xOffset, yOffset,
            1.0f - xOffset, yOffset,
            xOffset, 1.0f - yOffset,
            1.0f - xOffset, 1.0f - yOffset
    };

    glUseProgram(programId);
    //todo 使用VAO
    glVertexAttribPointer(vertexcoordAttrLoc, 2, GL_FLOAT, GL_FALSE, 0, OUTPUT_VIEW_VERTEX_COORD);
    glEnableVertexAttribArray(vertexcoordAttrLoc);
    glVertexAttribPointer(texcoordAttrLoc, 2, GL_FLOAT, GL_FALSE, 0, fitTexcoord);
    glEnableVertexAttribArray(texcoordAttrLoc);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, inputTexId);
    //设置纹理四周为黑色
    float borderColor[] = {0.0f, 0.0f, 0.0f, 0.0f};
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER_OES);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER_OES);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR_OES, borderColor);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR_OES, borderColor);
    glUniform1i(textureUniformLoc, 0);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisableVertexAttribArray(texcoordAttrLoc);
    glDisableVertexAttribArray(vertexcoordAttrLoc);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);
}

void GLSurfaceRender::renderWaterToTexture(GLuint inputTexId, int left, int top, int waterWidth, int waterHeight,
                                           GLuint outputTexId) {
    glViewport(left, top, waterWidth, waterHeight);
    if (!isInitProgram) {
        LOGE("renderWaterToTexture, renderToTexture error , isInitProgram = false");
        return;
    }
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, outputTexId, 0);
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        LOGE("renderWaterToTexture, failed to make complete framebuffer object %x", status);
        return;
    }
    float xOffset = 0, yOffset = 0;
    const GLfloat texcoord[] = {
            xOffset, yOffset,
            1.0f - xOffset, yOffset,
            xOffset, 1.0f - yOffset,
            1.0f - xOffset, 1.0f - yOffset
    };

    glUseProgram(programId);
    glVertexAttribPointer(vertexcoordAttrLoc, 2, GL_FLOAT, GL_FALSE, 0, OUTPUT_VIEW_VERTEX_COORD);
    glEnableVertexAttribArray(vertexcoordAttrLoc);
    glVertexAttribPointer(texcoordAttrLoc, 2, GL_FLOAT, GL_FALSE, 0, texcoord);
    glEnableVertexAttribArray(texcoordAttrLoc);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, inputTexId);
    glUniform1i(textureUniformLoc, 0);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisableVertexAttribArray(texcoordAttrLoc);
    glDisableVertexAttribArray(vertexcoordAttrLoc);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);
}
