//
// Created by dengchong on 2019-10-16.
//

#ifndef AUVIDEO_R2YCONVERTER_H
#define AUVIDEO_R2YCONVERTER_H

#include <GLTools.h>
#include <vector>
#include <AndroidLog.h>
#include "FFmpegUtils.h"

using namespace std;

class R2YConverter {
public:
    R2YConverter(int srcWidth, int srcHeight);

    ~R2YConverter();

    int convert(GLuint texId, uint8_t ***outData, int **outLineSize);

    int downloadDataFromGPU(GLuint texId, uint8_t ***outData);

private:

    uint8_t *srcData[4], *dstData[4];
    int srcLineSize[4], dstLineSize[4];
    int srcW, srcH;
    AVPixelFormat srcFormat{AV_PIX_FMT_RGBA}, dstFormat{AV_PIX_FMT_YUV420P};
    int ret{}, dstBufferSize{}, srcBufferSize{};
    bool initSuc{};
    uint8_t *downloadData;

    /**
     *
     * @return dstData size
     */
    int convert(uint8_t ***outData, int **outLineSize);

    void downloadDataFromGPU(GLuint texId);
};


#endif //AUVIDEO_R2YCONVERTER_H
