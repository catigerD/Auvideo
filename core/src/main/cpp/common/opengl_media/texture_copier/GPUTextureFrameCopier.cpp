//
// Created by dengchong on 2019-09-30.
//

#include "GPUTextureFrameCopier.h"

#define LOG_TAG "GPUTextureFrameCopier"

GPUTextureFrameCopier::GPUTextureFrameCopier()
        : TextureFrameCopier(NO_FILTER_VERTEX_SOURCE, GPU_FRAGMENT_SOURCE) {

}

GPUTextureFrameCopier::~GPUTextureFrameCopier() {

}

bool GPUTextureFrameCopier::init() {
    programId = loadProgram(vertexSource, fragmentSource);
    if (!programId) {
        return false;
    }
    verCoordsAttrLoc = static_cast<GLuint>(glGetAttribLocation(programId, "verCoords"));
    texCoordsAttrLoc = static_cast<GLuint>(glGetAttribLocation(programId, "texCoords"));
    verMatrixUniformLoc = static_cast<GLuint>(glGetUniformLocation(programId, "verMatrix"));
    texMatrixUniformLoc = static_cast<GLuint>(glGetUniformLocation(programId, "texMatrix"));
    gpuSamplerUniformLoc = static_cast<GLuint>(glGetUniformLocation(programId, "gpuSampler"));
    isInit = true;
    return true;
}

void GPUTextureFrameCopier::renderWithCoords(shared_ptr<TextureFrame> textureFrame, GLuint texId,
                                             GLfloat *vertexCoords, GLfloat *texCoords) {
    if (!isInit) {
        LOGE("GPUTextureFrameCopier::init() not success");
        return;
    }

    glBindTexture(GL_TEXTURE_2D, texId);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texId, 0);
    GLuint status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (GL_FRAMEBUFFER_COMPLETE != status) {
        LOGE("glCheckFramebufferStatus() not complete");
        return;
    }

    glUseProgram(programId);

    glVertexAttribPointer(verCoordsAttrLoc, 2, GL_FLOAT, GL_FALSE, 0, vertexCoords);
    glEnableVertexAttribArray(verCoordsAttrLoc);
    glVertexAttribPointer(texCoordsAttrLoc, 2, GL_FLOAT, GL_FALSE, 0, texCoords);
    glEnableVertexAttribArray(texCoordsAttrLoc);

    float verMatrix[4 * 4];
    matrixSetIdentityM(verMatrix);
    glUniformMatrix4fv(verMatrixUniformLoc, 1, GL_FALSE, verMatrix);
    float texMatrix[4 * 4];
    matrixSetIdentityM(texMatrix);
    glUniformMatrix4fv(texMatrixUniformLoc, 1, GL_FALSE, texMatrix);

    textureFrame->bindTexture(&gpuSamplerUniformLoc);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisableVertexAttribArray(texCoordsAttrLoc);
    glDisableVertexAttribArray(verCoordsAttrLoc);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GPUTextureFrameCopier::destroy() {
    if (programId) {
        glDeleteProgram(programId);
    }
}