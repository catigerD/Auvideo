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

    void convert(GLuint texId, int width, int height, uint8_t *data);

private:

    void load(GLuint texId, int width, int height, uint8_t *data);
};


#endif //AUVIDEO_R2YCONVERTER_H
