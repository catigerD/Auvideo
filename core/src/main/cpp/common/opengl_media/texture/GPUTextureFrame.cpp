//
// Created by dengchong on 2019-09-27.
//

#include "GPUTextureFrame.h"

void GPUTextureFrame::initTexture() {
    glGenTextures(1, &texId);
    glBindTexture(GL_TEXTURE_EXTERNAL_OES, texId);
    glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_EXTERNAL_OES, 0);
}

void GPUTextureFrame::updateTexImage() {
    //调用 Java 层 SurfaceTexture.updateTexImage 更新纹理
}

void GPUTextureFrame::bindTexture(GLuint *samplerLoc) {
    glBindTexture(GL_TEXTURE_EXTERNAL_OES, texId);
    glUniform1i(*samplerLoc, 0);
}
