//
// Created by dengchong on 2019-10-23.
//

#ifndef CPP_FFMPEGUTILS_H
#define CPP_FFMPEGUTILS_H

#include "FFmpegAlloc.h"
#include "AndroidLog.h"

struct FFmpegUtils {
    static bool convert(uint8_t *srcData[4], int srcLineSize[4], AVPixelFormat srcPixFmt, int srcW, int srcH,
                        uint8_t *dstData[4], int dstLineSize[4], AVPixelFormat dstPixFmt, int dstW, int dstH);
};


#endif //CPP_FFMPEGUTILS_H
