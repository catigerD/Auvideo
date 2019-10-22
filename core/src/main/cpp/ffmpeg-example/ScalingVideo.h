//
// Created by dengchong on 2019-10-22.
//

#ifndef CPP_SCALINGVIDEO_H
#define CPP_SCALINGVIDEO_H

#include "FFmpegAlloc.h"
#include <fstream>
#include "AndroidLog.h"

class ScalingVideo {
public:

    ScalingVideo(const string &fileName) : stream(fileName) {

    }

    void convert(const char* dstSize);

private:
    ofstream stream;

    shared_ptr<SwsContext> swsContext;

    uint8_t *srcData[4], *dstData[4];
    int srcLineSize[4], dstLineSize[4];
    int srcW{320}, srcH{240}, dstW, dstH;
    AVPixelFormat srcPixFmt{AV_PIX_FMT_YUV420P}, dstPixFmt{AV_PIX_FMT_RGB24};
    int dstBufferSize{};
    int ret{};

    void fillYUVImage(uint8_t *data[4], int lineSize[4], int width, int height, int frameIndex);

    void release();
};


#endif //CPP_SCALINGVIDEO_H
