//
// Created by dengchong on 2019-10-14.
//

#ifndef AUVIDEO_VIDEOFRAME_H
#define AUVIDEO_VIDEOFRAME_H

#include <vector>
#include <cstdint>
#include <chrono>

struct VideoFrame {

    uint8_t **data;
    int *lineSize;
    int bufferSize;
    std::chrono::milliseconds timeMills{};
    int duration{};
    int64_t pts{};
    int64_t dts{};
};


#endif //AUVIDEO_VIDEOFRAME_H
