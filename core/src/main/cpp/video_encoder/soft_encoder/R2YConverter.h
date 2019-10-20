//
// Created by dengchong on 2019-10-16.
//

#ifndef AUVIDEO_R2YCONVERTER_H
#define AUVIDEO_R2YCONVERTER_H

#include <GLTools.h>
#include <vector>
#include <AndroidLog.h>

using namespace std;

class R2YConverter {
public:
    R2YConverter() = default;

    ~R2YConverter() = default;

    void convert(GLuint texId, int width, int height, vector<uint8_t> &data);

private:
    vector<uint8_t> rgbaData;
    vector<uint8_t> yuv420pData;

    void load(GLuint texId, int width, int height, vector<uint8_t> &data);
};


#endif //AUVIDEO_R2YCONVERTER_H
