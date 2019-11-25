//
// Created by dengchong on 2019-09-30.
//

#include "GPUTextureFrameCopier.h"

#define LOG_TAG "GPUTextureFrameCopier"

GPUTextureFrameCopier::GPUTextureFrameCopier(int degress, int viewWidth, int viewHeight)
        : viewWidth(degress == 90 || degress == 270 ? viewHeight : viewWidth),
          viewHeight(degress == 90 || degress == 270 ? viewWidth : viewHeight),
          TextureFrameCopier(NO_FILTER_VERTEX_SOURCE, GPU_FRAGMENT_SOURCE) {

}

GPUTextureFrameCopier::~GPUTextureFrameCopier() = default;

bool GPUTextureFrameCopier::init() {
    programId = loadProgram(vertexSource, fragmentSource);
    LOGI("GPUTextureFrameCopier::init() programId : %d", programId);
    if (!programId) {
        return false;
    }
    gpuSamplerUniformLoc = static_cast<GLuint>(glGetUniformLocation(programId, "gpuSampler"));
    isInit = true;
    return isInit;
}

void GPUTextureFrameCopier::renderWithCoords(shared_ptr<TextureFrame> textureFrame, GLuint texId,
                                             const GLfloat *vertexCoords,
                                             const GLfloat *texCoords) {
    if (!isInit) {
        LOGE("GPUTextureFrameCopier::init() not success");
        return;
    }

    glViewport(0, 0, viewWidth, viewHeight);
    LOGI("GPUTextureFrameCopier::renderWithCoords viewWidth : %d, view Height : %d", viewWidth, viewHeight);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texId, 0);
    checkGLError("glFramebufferTexture2D");
    GLuint status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (GL_FRAMEBUFFER_COMPLETE != status) {
        LOGE("glCheckFramebufferStatus() not complete， status : %d ， textureid : %d", status,
             texId);
        return;
    }

    glUseProgram(programId);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, vertexCoords);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, texCoords);
    glEnableVertexAttribArray(1);

    textureFrame->bindTexture(&gpuSamplerUniformLoc);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GPUTextureFrameCopier::destroy() {
    if (programId) {
        glDeleteProgram(programId);
    }
}
