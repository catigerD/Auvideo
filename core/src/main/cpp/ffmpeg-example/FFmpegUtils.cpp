//
// Created by dengchong on 2019-10-23.
//

#include "FFmpegUtils.h"

#define LOG_TAG "FFmpegUtils"

bool
FFmpegUtils::convert(uint8_t **srcData, int *srcLineSize, AVPixelFormat srcPixFmt, int srcW, int srcH,
                     uint8_t **dstData, int *dstLineSize, AVPixelFormat dstPixFmt, int dstW, int dstH) {
    shared_ptr<SwsContext> swsContext = FFmpegAlloc::getSwsContext(srcW, srcH, srcPixFmt, dstW, dstH, dstPixFmt);
    if (!swsContext) {
        LOGE("Impossible to create scale context for the conversion, fmt : %s s: %dx%d -> fmt : %s s : %dx%d",
             av_get_pix_fmt_name(srcPixFmt), srcW, srcH,
             av_get_pix_fmt_name(dstPixFmt), dstW, dstH);
        return false;
    }
    sws_scale(swsContext.get(), srcData, srcLineSize, 0, srcH, dstData, dstLineSize);
    return true;
}
