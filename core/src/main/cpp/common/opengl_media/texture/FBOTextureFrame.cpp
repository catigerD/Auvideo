//
// Created by dengchong on 2019-09-30.
//

#include "FBOTextureFrame.h"

#include <GLES2/gl2ext.h>

#define LOG_TAG "FBOTextureFrame"

using namespace std;

void FBOTextureFrame::initTexture() {
    glGenTextures(1, &texId);
    glBindTexture(GL_TEXTURE_2D, texId);
    float borderColor[] = {0.0f, 0.0f, 0.0f, 1.0f};
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER_OES);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR_OES, borderColor);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER_OES);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR_OES, borderColor);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //width height = 0 会报GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT错误
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 nullptr);
    glBindTexture(GL_TEXTURE_2D, 0);
    glGenFramebuffers(1, &fbo);
}
