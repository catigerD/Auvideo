#include <utility>

//
// Created by dengchong on 2019-09-27.
//

#include "ImageTextureFrame.h"

using namespace std;

ImageTextureFrame::ImageTextureFrame(string imagePath)
        : validPath(!imagePath.empty()),
          imagePath(std::move(imagePath)) {
}

ImageTextureFrame::~ImageTextureFrame() {
    stbi_image_free(data);
}

void ImageTextureFrame::initTexture() {
    if (!validPath) {
        return;
    }
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
    if (!validPath) {
        return;
    }
    if (!data) {
        data = stbi_load(imagePath.c_str(), &width, &height, &channel, 0);
    }
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE,
                 data);
    glBindTexture(GL_TEXTURE_2D, 0);
}
