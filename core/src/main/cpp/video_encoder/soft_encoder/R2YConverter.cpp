//
// Created by dengchong on 2019-10-16.
//

#include <cstring>
#include "R2YConverter.h"

#define LOG_TAG "R2YConverter"

R2YConverter::R2YConverter(int srcWidth, int srcHeight)
        : srcW(srcWidth), srcH(srcHeight) {
    ret = av_image_alloc(srcData, srcLineSize, srcW, srcH, srcFormat, 1);
    if (ret < 0) {
        LOGE("Could not allocate source image");
        return;
    }
    srcBufferSize = ret;
    LOGI("srcW %d, srcData : srcLineSize[] %d , %d , %d, %d", srcW, srcLineSize[0], srcLineSize[1], srcLineSize[2],
         srcLineSize[3]);
    ret = av_image_alloc(dstData, dstLineSize, srcW, srcH, dstFormat, 16);
    if (ret < 0) {
        LOGE("Could not allocate destination image");
        return;
    }
    dstBufferSize = ret;
    downloadData = new uint8_t[srcW * srcH * 4];
    initSuc = true;
}

R2YConverter::~R2YConverter() {
    av_freep(srcData);
    av_freep(dstData);
    delete[]downloadData;
}


int R2YConverter::convert(GLuint texId, uint8_t ***outData, int **outLineSize) {
    downloadDataFromGPU(texId);
    return convert(outData, outLineSize);
}

int R2YConverter::downloadDataFromGPU(GLuint texId, uint8_t ***outData) {
    downloadDataFromGPU(texId);
    memcpy(&srcData[0][0], downloadData, srcW * srcH * 4);
    *outData = srcData;
    return srcBufferSize;
}

/*-------------------------------private-------------------------------*/

void R2YConverter::downloadDataFromGPU(GLuint texId) {
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texId, 0);
    checkGLError("R2YConverter : glFramebufferTexture2D");
    auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        LOGE("failed to make complete framebuffer object %x", status);
        return;
    }
    //glReadPixels 只有 3 种可用格式。GL_RGBA,GL_RGBA_INTEGER。。。
    glReadPixels(0, 0, srcW, srcH, GL_RGBA, GL_UNSIGNED_BYTE, downloadData);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);
}

int R2YConverter::convert(uint8_t ***outData, int **outLineSize) {
    if (!initSuc) {
        return -1;
    }
    memcpy(&srcData[0][0], downloadData, srcW * srcH * 4);
    FFmpegUtils::convert(srcData, srcLineSize, srcFormat, srcW, srcH, dstData, dstLineSize, dstFormat, srcW, srcH);
    *outData = dstData;
    *outLineSize = dstLineSize;
    return dstBufferSize;
}

