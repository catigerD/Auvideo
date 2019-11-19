#include <utility>

//
// Created by dengchong on 2019-10-14.
//

#ifndef AUVIDEO_VIDEOENCODERADAPTER_H
#define AUVIDEO_VIDEOENCODERADAPTER_H

#include <string>
#include <fstream>
#include "GLSurfaceRender.h"
#include "EGLCore.h"

class VideoEncoderAdapter {
public:
    VideoEncoderAdapter(std::string path, int width, int height, int bitRate, int frameRate) :
            filePath(std::move(path)),
            encodeWidth(width),
            encodeHeight(height),
            encodeBitRate(bitRate),
            encodeFrameRate(frameRate),
            render(std::make_shared<GLSurfaceRender>(width, height)) {
    }

    virtual ~VideoEncoderAdapter() = default;

    virtual void createEncoder(std::shared_ptr<EGLCore> core, GLuint inputTexId) = 0;

    virtual void encode() = 0;

    virtual void destroyEncoder() = 0;

protected:
    const std::string filePath;
    int encodeWidth;
    int encodeHeight;
    int encodeBitRate;
    int encodeFrameRate;
    std::shared_ptr<GLSurfaceRender> render;
};


#endif //AUVIDEO_VIDEOENCODERADAPTER_H
