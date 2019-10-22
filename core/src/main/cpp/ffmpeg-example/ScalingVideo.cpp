//
// Created by dengchong on 2019-10-22.
//

#include "ScalingVideo.h"

#define LOG_TAG "ScalingVideo"

void ScalingVideo::fillYUVImage(uint8_t **data, int *lineSize, int width, int height, int frameIndex) {
    int x, y;
    /*Y*/
    for (y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            data[0][y * lineSize[0] + x] = x + y + frameIndex * 3;
        }
    }
    /*Cb and Cr*/
    for (y = 0; y < height / 2; ++y) {
        for (int x = 0; x < width / 2; ++x) {
            data[1][y * lineSize[1] + x] = 128 + y + frameIndex * 2;
            data[2][y * lineSize[2] + x] = 64 + x + frameIndex * 5;
        }
    }
}

void ScalingVideo::convert(const char *dstSize) {
    if (av_parse_video_size(&dstW, &dstH, dstSize)) {
        LOGE("Invalid size %s , must be in the form WxH or a valid size abbreviation", dstSize);
        return;
    }
    /*create scaling context*/
    swsContext = FFmpegAlloc::getSwsContext(srcW, srcW, srcPixFmt, dstW, dstH, dstPixFmt);
    if (!swsContext) {
        LOGE("Impossible to create scale context for the conversion, fmt : %s s: %dx%d -> fmt : %s s : %dx%d",
             av_get_pix_fmt_name(srcPixFmt), srcW, srcH,
             av_get_pix_fmt_name(dstPixFmt), dstW, dstH);
        ret = AVERROR(EINVAL);
        release();
        return;
    }
    /* allocate source and destination image buffers*/
    ret = av_image_alloc(srcData, srcLineSize, srcW, srcH, srcPixFmt, 16);
    if (ret < 0) {
        LOGE("Could not allocate source image");
        release();
        return;
    }
    /* buffer is going to be written to rawvideo file, no alignment*/
    ret = av_image_alloc(dstData, dstLineSize, dstW, dstH, dstPixFmt, 1);
    if (ret < 0) {
        LOGE("Could not allocate destination image");
        release();
        return;
    }
    dstBufferSize = ret;
    for (int i = 0; i < 100; ++i) {
        /* generate synthetic video*/
        fillYUVImage(srcData, srcLineSize, srcW, srcH, i);
        /* convert to destination format*/
        sws_scale(swsContext.get(), srcData, srcLineSize, 0, srcH, dstData, dstLineSize);
        /* write scaled image to file*/
        stream.write(reinterpret_cast<const char *>(dstData[0]), dstBufferSize);
    }
    LOGI("Scaling succeeded , play the output file with the cmd : ffplay -f rawvideo -pix_fmt %s -video_size %dx%d",
         av_get_pix_fmt_name(dstPixFmt), dstW, dstH);
}

void ScalingVideo::release() {
    stream.close();
    av_freep(&srcData[0]);
    av_freep(&dstData[0]);
}
