//
// Created by dengchong on 2019-11-09.
//

#ifndef CPP_VIDEOX264MUXER_H
#define CPP_VIDEOX264MUXER_H

#include <memory>
#include <string>

#include "FFmpegAlloc.h"

class VideoX264Muxer {

public:

    VideoX264Muxer(std::string fileName, std::shared_ptr<AVCodecContext> codecContext);

    bool initAndWriteHeader();

    void writePacket(const std::shared_ptr<AVPacket> &packet);

    void writeTrailer();

private:
    const std::string fileName;
    std::shared_ptr<AVCodecContext> codecContext;
    std::shared_ptr<AVFormatContext> formatContext;
    std::shared_ptr<AVStream> videoStream;

    bool initSuc{};
private:
    bool addStream();
};


#endif //CPP_VIDEOX264MUXER_H
