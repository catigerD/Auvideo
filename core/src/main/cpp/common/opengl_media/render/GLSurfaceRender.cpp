//
// Created by dengchong on 2019-09-25.
//

#include "GLSurfaceRender.h"

#define LOG_TAG "GLSurfaceRender"

using namespace std;

void GLSurfaceRender::init() {
    programId = loadProgram(RenderConfig::VERTEX_SOURCE.c_str(), RenderConfig::FRAGMENT_SOURCE.c_str());
    if (!programId) {
        LOGE("Could not create Program");
        return;
    }
    auto sampler2DIndex = static_cast<GLuint>(glGetAttribLocation(programId, "texture"));
    glUniform1i(sampler2DIndex, 0);

    glGenBuffers(2, VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, RenderConfig::VERTEX_PTR_SIZE, RenderConfig::VERTEX_COORD.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, RenderConfig::VERTEX_PTR_SIZE, RenderConfig::TEXTURE_COORD.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glEnableVertexAttribArray(RenderConfig::VERTEX_POS_INDEX);
    glVertexAttribPointer(RenderConfig::VERTEX_POS_INDEX, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glEnableVertexAttribArray(RenderConfig::VERTEX_TEX_INDEX);
    glVertexAttribPointer(RenderConfig::VERTEX_TEX_INDEX, 2, GL_FLOAT, GL_FALSE, 0, 0);
    //***需要调用 glBindBuffer(GL_ARRAY_BUFFER, 0) 解绑？不然会黑屏。。。
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glGenSamplers(1, &SMO);
    glBindSampler(0, SMO);
    glSamplerParameteri(SMO, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glSamplerParameteri(SMO, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //设置纹理四周为黑色
    float borderColor[] = {0.0f, 0.0f, 0.0f, 0.0f};
    glSamplerParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER_OES);
    glSamplerParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER_OES);
    glSamplerParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR_OES, borderColor);
    glSamplerParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR_OES, borderColor);
    glBindSampler(0, 0);
}

void GLSurfaceRender::resetRenderSize(int left, int top, int width, int height) {
    this->left = left;
    this->top = top;
    this->viewWidth = width;
    this->viewHeight = height;
}

void GLSurfaceRender::renderToView(GLuint texId) {
    renderToView(texId, viewWidth, viewHeight);
}

void GLSurfaceRender::renderToView(GLuint texId, int renderWidth, int renderHeight) {
    if (!programId) {
        LOGE("renderToView error , isInitProgram = false");
        return;
    }

    glViewport(left, top, renderWidth, renderHeight);

    glUseProgram(programId);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, RenderConfig::VERTEX_PTR_SIZE, RenderConfig::TEXTURE_COORD.data());
    glBindTexture(GL_TEXTURE_2D, texId);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void GLSurfaceRender::renderToViewWithAutoFit(GLuint texId, int renderWidth, int renderHeight,
                                              int texWidth, int texHeight) {
    if (!programId) {
        LOGE("renderToView error , isInitProgram = false");
        return;
    }

    glViewport(0, 0, renderWidth, renderHeight);

    array<GLfloat, RenderConfig::VERTEX_SIZE> texCoord{};
    RenderConfig::getFitTexCoord(renderWidth, renderHeight, texWidth, texHeight, texCoord);

    glUseProgram(programId);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, RenderConfig::VERTEX_PTR_SIZE, texCoord.data());
    glBindTexture(GL_TEXTURE_2D, texId);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void GLSurfaceRender::renderToViewWithAutoFill(GLuint texId, int renderWidth, int renderHeight,
                                               int texWidth, int texHeight) {
    if (!programId) {
        LOGE("renderToView error , isInitProgram = false");
        return;
    }

    glViewport(0, 0, renderWidth, renderHeight);

    array<GLfloat, RenderConfig::VERTEX_SIZE> texCoord{};
    RenderConfig::getFillTexCoord(renderWidth, renderHeight, texWidth, texHeight, texCoord);

    glUseProgram(programId);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, RenderConfig::VERTEX_PTR_SIZE, texCoord.data());
    glBindTexture(GL_TEXTURE_2D, texId);
    glBindSampler(0, SMO);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindSampler(0, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void GLSurfaceRender::renderToTexture(GLuint inputTexId, GLuint outputTexId) {
    if (!programId) {
        LOGE("renderToTexture error , isInitProgram = false");
        return;
    }

    glViewport(left, top, viewWidth, viewHeight);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, outputTexId, 0);
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        LOGE("failed to make complete framebuffer object %x", status);
        return;
    }

    glUseProgram(programId);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, RenderConfig::VERTEX_PTR_SIZE, RenderConfig::TEXTURE_COORD.data());
    glBindTexture(GL_TEXTURE_2D, inputTexId);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);
}

void GLSurfaceRender::renderToVFlipTexture(GLuint inputTexId, GLuint outputTexId) {
    if (!programId) {
        LOGE("renderToTexture error , isInitProgram = false");
        return;
    }

    glViewport(left, top, viewWidth, viewHeight);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, outputTexId, 0);
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        LOGE("failed to make complete framebuffer object %x", status);
        return;
    }

    glUseProgram(programId);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, RenderConfig::VERTEX_PTR_SIZE, RenderConfig::V_FLIP_TEXTURE_COORD.data());
    glBindTexture(GL_TEXTURE_2D, inputTexId);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);
}

void GLSurfaceRender::renderToAutoFillTexture(GLuint inputTexId, int texWidth, int texHeight,
                                              GLuint outputTexId) {
    if (!programId) {
        LOGE("renderToTexture error , isInitProgram = false");
        return;
    }

    array<GLfloat, RenderConfig::VERTEX_SIZE> texCoords{};
    RenderConfig::getFillTexCoord(viewWidth, viewHeight, texWidth, texHeight, texCoords);

    glViewport(left, top, viewWidth, viewHeight);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, outputTexId, 0);
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        LOGE("failed to make complete framebuffer object %x", status);
        return;
    }

    glUseProgram(programId);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, RenderConfig::VERTEX_PTR_SIZE, texCoords.data());
    glBindTexture(GL_TEXTURE_2D, inputTexId);
    glBindSampler(0, SMO);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glBindSampler(0, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);
}

void GLSurfaceRender::renderToAutoFitTexture(GLuint inputTexId, int texWidth, int texHeight,
                                             GLuint outputTexId) {
    if (!programId) {
        LOGE("renderToTexture error , isInitProgram = false");
        return;
    }

    glViewport(left, top, viewWidth, viewHeight);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, outputTexId, 0);
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        LOGE("failed to make complete framebuffer object %x", status);
        return;
    }

    array<GLfloat, RenderConfig::VERTEX_SIZE> texCoord{};
    RenderConfig::getFitTexCoord(viewWidth, viewHeight, texWidth, texHeight, texCoord);

    glUseProgram(programId);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, RenderConfig::VERTEX_PTR_SIZE, texCoord.data());
    glBindTexture(GL_TEXTURE_2D, inputTexId);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);
}

void GLSurfaceRender::renderWaterToTexture(GLuint inputTexId, int left, int top, int waterWidth,
                                           int waterHeight,
                                           GLuint outputTexId) {
    if (!programId) {
        LOGE("renderWaterToTexture, renderToTexture error , isInitProgram = false");
        return;
    }

    glViewport(left, top, waterWidth, waterHeight);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, outputTexId, 0);
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        LOGE("renderWaterToTexture, failed to make complete framebuffer object %x", status);
        return;
    }

    glUseProgram(programId);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, RenderConfig::VERTEX_PTR_SIZE, RenderConfig::TEXTURE_COORD.data());
    glBindTexture(GL_TEXTURE_2D, inputTexId);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);
}
