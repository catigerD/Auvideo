//
// Created by dengchong on 2019-09-25.
//

#include "GLSurfaceRender.h"

#define LOG_TAG "GLSurfaceRender"

GLSurfaceRender::GLSurfaceRender(int width, int height, const char *vertexSource,
                                 const char *fragmentSource)
        : width(width),
          height(height),
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
    this->width = width;
    this->height = height;
}

void GLSurfaceRender::renderToView(GLuint texId) {
    glViewport(left, top, width, height);
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

void GLSurfaceRender::renderToView(GLuint texId, int screenWidth, int screenHeight) {
    glViewport(0, 0, screenWidth, screenHeight);
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

void GLSurfaceRender::renderToViewWithAutoFill(GLuint texId, int screenWidth, int screenHeight,
                                               int texWidth, int texHeight) {
    glViewport(0, 0, screenWidth, screenHeight);
    if (!isInitProgram) {
        LOGE("renderToView error , isInitProgram = false");
        return;
    }

    float texAspectRatio = static_cast<float>(texHeight) / texWidth;
    float screenAspectRatio = static_cast<float>(screenHeight) / screenWidth;
    bool changeX = true;
    float ratio = 1.0;
    if (texAspectRatio > screenAspectRatio) {
        int fitHeight = static_cast<int>(static_cast<float >(screenHeight) / screenWidth *
                                         texWidth);
        ratio = static_cast<float>(texHeight - fitHeight) / (2 * texHeight);
        changeX = false;
    } else {
        int fitWidth = static_cast<int>(static_cast<float>(screenWidth) / screenHeight * texHeight);
        ratio = static_cast<float>(texWidth - fitWidth) / (2 * texWidth);
        changeX = true;
    }
    glUseProgram(programId);
    //todo 使用 VAO
    glVertexAttribPointer(vertexcoordAttrLoc, 2, GL_FLOAT, GL_FALSE, 0, OUTPUT_VIEW_VERTEX_COORD);
    glEnableVertexAttribArray(vertexcoordAttrLoc);
    if (changeX) {
        const GLfloat fitTexcoord[] = {
                ratio, 0.0f,
                1.0f - ratio, 0.0f,
                ratio, 1.0f,
                1.0f - ratio, 1.0f
        };
        glVertexAttribPointer(texcoordAttrLoc, 2, GL_FLOAT, GL_FALSE, 0, fitTexcoord);
    } else {
        const GLfloat fitTexcoord[] = {
                0.0f, ratio,
                1.0f, ratio,
                0.0f, 1.0f - ratio,
                1.0f, 1.0f - ratio
        };
        glVertexAttribPointer(texcoordAttrLoc, 2, GL_FLOAT, GL_FALSE, 0, fitTexcoord);
    }
    glEnableVertexAttribArray(texcoordAttrLoc);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texId);
    glUniform1i(textureUniformLoc, 0);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisableVertexAttribArray(texcoordAttrLoc);
    glDisableVertexAttribArray(vertexcoordAttrLoc);
}

void GLSurfaceRender::renderToViewWithAutoFit(GLuint texId, int screenWidth, int screenHeight,
                                              int texWidth, int texHeight) {
    glViewport(0, 0, screenWidth, screenHeight);
    if (!isInitProgram) {
        LOGE("renderToView error , isInitProgram = false");
        return;
    }

    float texAspectRatio = static_cast<float>(texHeight) / texWidth;
    float screenAspectRatio = static_cast<float>(screenHeight) / screenWidth;
    bool changeX = true;
    float ratio = 1.0;
    if (texAspectRatio > screenAspectRatio) {
        int fitWidth = static_cast<int>(static_cast<float >(screenHeight) / texHeight *
                                        texWidth);
        ratio = static_cast<float>(fitWidth - screenWidth) / (2 * fitWidth);
        changeX = true;
    } else {
        int fitHeight = static_cast<int>(static_cast<float>(screenWidth) / texWidth *
                                         texHeight);
        ratio = static_cast<float>(fitHeight - screenHeight) / (2 * fitHeight);
        changeX = false;
    }
    glUseProgram(programId);
    //todo 使用 VAO
    glVertexAttribPointer(vertexcoordAttrLoc, 2, GL_FLOAT, GL_FALSE, 0, OUTPUT_VIEW_VERTEX_COORD);
    glEnableVertexAttribArray(vertexcoordAttrLoc);
    if (changeX) {
        const GLfloat fitTexcoord[] = {
                ratio, 0.0f,
                1.0f - ratio, 0.0f,
                ratio, 1.0f,
                1.0f - ratio, 1.0f
        };
        glVertexAttribPointer(texcoordAttrLoc, 2, GL_FLOAT, GL_FALSE, 0, fitTexcoord);
    } else {
        const GLfloat fitTexcoord[] = {
                0.0f, ratio,
                1.0f, ratio,
                0.0f, 1.0f - ratio,
                1.0f, 1.0f - ratio
        };
        glVertexAttribPointer(texcoordAttrLoc, 2, GL_FLOAT, GL_FALSE, 0, fitTexcoord);
    }
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
    glViewport(left, top, width, height);
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
    glViewport(left, top, width, height);
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

void GLSurfaceRender::renderToAutoFitTexture(GLuint inputTexId, int inputWidth, int inputHeight,
                                             GLuint outputTexId) {
    glViewport(left, top, width, height);
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

    float texAspectRatio = static_cast<float>(inputHeight) / inputWidth;
    float screenAspectRatio = static_cast<float>(height) / width;
    bool changeX = true;
    float ratio = 1.0;
    if (texAspectRatio > screenAspectRatio) {
        int fitWidth = static_cast<int>(static_cast<float >(height) / inputHeight *
                                        inputWidth);
        ratio = static_cast<float>(fitWidth - width) / (2 * fitWidth);
        changeX = true;
    } else {
        int fitHeight = static_cast<int>(static_cast<float>(width) / inputWidth *
                                         inputHeight);
        ratio = static_cast<float>(fitHeight - height) / (2 * fitHeight);
        changeX = false;
    }

    glUseProgram(programId);
    //todo 使用VAO
    glVertexAttribPointer(vertexcoordAttrLoc, 2, GL_FLOAT, GL_FALSE, 0, OUTPUT_VIEW_VERTEX_COORD);
    glEnableVertexAttribArray(vertexcoordAttrLoc);
    if (changeX) {
        const GLfloat fitTexcoord[] = {
                ratio, 0.0f,
                1.0f - ratio, 0.0f,
                ratio, 1.0f,
                1.0f - ratio, 1.0f
        };
        glVertexAttribPointer(texcoordAttrLoc, 2, GL_FLOAT, GL_FALSE, 0, fitTexcoord);
    } else {
        const GLfloat fitTexcoord[] = {
                0.0f, ratio,
                1.0f, ratio,
                0.0f, 1.0f - ratio,
                1.0f, 1.0f - ratio
        };
        glVertexAttribPointer(texcoordAttrLoc, 2, GL_FLOAT, GL_FALSE, 0, fitTexcoord);
    }
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

void GLSurfaceRender::renderToAutoFillTexture(GLuint inputTexId, int inputWidth, int inputHeight,
                                              GLuint outputTexId) {
    glViewport(left, top, width, height);
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

    float texAspectRatio = static_cast<float>(inputHeight) / inputWidth;
    float screenAspectRatio = static_cast<float>(height) / width;
    bool changeX = true;
    float ratio = 1.0;
    if (texAspectRatio > screenAspectRatio) {
        int fitHeight = static_cast<int>(static_cast<float >(height) / width *
                                         inputWidth);
        ratio = static_cast<float>(inputHeight - fitHeight) / (2 * inputHeight);
        changeX = false;
    } else {
        int fitWidth = static_cast<int>(static_cast<float>(width) / height * inputHeight);
        ratio = static_cast<float>(inputWidth - fitWidth) / (2 * inputWidth);
        changeX = true;
    }

    glUseProgram(programId);
    //todo 使用VAO
    glVertexAttribPointer(vertexcoordAttrLoc, 2, GL_FLOAT, GL_FALSE, 0, OUTPUT_VIEW_VERTEX_COORD);
    glEnableVertexAttribArray(vertexcoordAttrLoc);
    if (changeX) {
        const GLfloat fitTexcoord[] = {
                ratio, 0.0f,
                1.0f - ratio, 0.0f,
                ratio, 1.0f,
                1.0f - ratio, 1.0f
        };
        glVertexAttribPointer(texcoordAttrLoc, 2, GL_FLOAT, GL_FALSE, 0, fitTexcoord);
    } else {
        const GLfloat fitTexcoord[] = {
                0.0f, ratio,
                1.0f, ratio,
                0.0f, 1.0f - ratio,
                1.0f, 1.0f - ratio
        };
        glVertexAttribPointer(texcoordAttrLoc, 2, GL_FLOAT, GL_FALSE, 0, fitTexcoord);
    }
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
