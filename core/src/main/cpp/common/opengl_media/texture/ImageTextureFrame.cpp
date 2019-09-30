//
// Created by dengchong on 2019-09-27.
//

#include "ImageTextureFrame.h"

ImageTextureFrame::ImageTextureFrame(const char *imagePath, int &width, int &height, int &channel)
        : imagePath(imagePath),
          width(width),
          height(height),
          channel(channel) {
}

ImageTextureFrame::~ImageTextureFrame() {
    stbi_image_free(data);
}

void ImageTextureFrame::initTexture() {
    glGenTextures(1, &texId);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void ImageTextureFrame::updateTexImage() {
    if (!data) {
        data = stbi_load(imagePath, &width, &height, &channel, 0);
    }
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE,
                 data);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void ImageTextureFrame::bindTexture(GLuint *samplerLoc) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texId);
    glUniform1i(*samplerLoc, 0);
}
