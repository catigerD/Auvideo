//
// Created by dengchong on 2019-10-14.
//

#ifndef AUVIDEO_VIDEOENCODERADAPTER_H
#define AUVIDEO_VIDEOENCODERADAPTER_H

#include <string>
#include <fstream>
#include "GLSurfaceRender.h"
#include "EGLCore.h"

using namespace std;

class VideoEncoderAdapter {
public:
    VideoEncoderAdapter(const string &path, int width, int height, int bitRate, int frameRate) :
            filePath(path),
            encodeWidth(width),
            encodeHeight(height),
            encodeBitRate(bitRate),
            encodeFrameRate(frameRate),
            render(make_shared<GLSurfaceRender>(width, height)) {
    }

    virtual ~VideoEncoderAdapter() = default;

    virtual void createEncoder(shared_ptr<EGLCore> core, GLuint inputTexId) = 0;

    virtual void encode() = 0;

    virtual void destroyEncoder() = 0;

protected:
    const string filePath;
    int encodeWidth;
    int encodeHeight;
    int encodeBitRate;
    int encodeFrameRate;
    shared_ptr<GLSurfaceRender> render;
};


#endif //AUVIDEO_VIDEOENCODERADAPTER_H
