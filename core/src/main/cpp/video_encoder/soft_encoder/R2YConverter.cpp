//
// Created by dengchong on 2019-10-16.
//

#include <cstring>
#include "R2YConverter.h"

#define LOG_TAG "R2YConverter"

uint8_t clip_value(uint8_t x, uint8_t min_val, uint8_t max_val) {
    if (x > max_val) {
        return max_val;
    } else if (x < min_val) {
        return min_val;
    } else {
        return x;
    }
}

//RGB to YUV420
bool RGB24_TO_YUV420(uint8_t *RgbBuf, int w, int h, uint8_t *yuvBuf) {
    uint8_t *ptrY, *ptrU, *ptrV, *ptrRGB;
    memset(yuvBuf, 0, static_cast<size_t>(w * h * 3 / 2));
    ptrY = yuvBuf;
    ptrU = yuvBuf + w * h;
    ptrV = ptrU + (w * h * 1 / 4);
    uint8_t y, u, v, r, g, b;
    for (int j = 0; j < h; j++) {
        ptrRGB = RgbBuf + w * j * 4;
        for (int i = 0; i < w; i++) {
            r = *(ptrRGB++);
            g = *(ptrRGB++);
            b = *(ptrRGB++);
            y = static_cast<uint8_t>(((66 * r + 129 * g + 25 * b + 128) >> 8) + 16);
            u = static_cast<uint8_t>(((-38 * r - 74 * g + 112 * b + 128) >> 8) + 128);
            v = static_cast<uint8_t>(((112 * r - 94 * g - 18 * b + 128) >> 8) + 128);
            *(ptrY++) = clip_value(y, 0, 255);
            if (j % 2 == 0 && i % 2 == 0) {
                *(ptrU++) = clip_value(u, 0, 255);
            } else {
                if (i % 2 == 0) {
                    *(ptrV++) = clip_value(v, 0, 255);
                }
            }
        }
    }
    return true;
}

void R2YConverter::convert(GLuint texId, int width, int height, uint8_t *data) {
    uint8_t *rgbaData = new uint8_t[width * height * 4];
    load(texId, width, height, rgbaData);
    RGB24_TO_YUV420(rgbaData, width, height, data);
    delete[]rgbaData;
}

void R2YConverter::load(GLuint texId, int width, int height, uint8_t *data) {
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texId, 0);
    checkGLError("R2YConverter : glFramebufferTexture2D");
    auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        LOGE("failed to make complete framebuffer object %x", status);
        return;
    }
    //glReadPixels 只有 3 种可用格式。GL_RGBA,GL_RGBA_INTEGER。。。
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);
}

