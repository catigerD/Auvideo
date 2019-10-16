//
// Created by dengchong on 2019-10-14.
//

#ifndef AUVIDEO_VIDEOFRAME_H
#define AUVIDEO_VIDEOFRAME_H

#include <vector>

using namespace std;

struct VideoFrame {
    vector<uint8_t> data;
    int timeMills;
    int duration;
    int64_t pts;
    int64_t dts;
};


#endif //AUVIDEO_VIDEOFRAME_H
