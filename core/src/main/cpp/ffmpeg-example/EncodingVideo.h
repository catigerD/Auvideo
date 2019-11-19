//
// Created by dengchong on 2019-10-18.
//

#ifndef CPP_ENCODEEXAMPLER_H
#define CPP_ENCODEEXAMPLER_H

#include "FFmpegAlloc.h"
#include <memory>
#include "AndroidLog.h"
#include <fstream>
#include <vector>

class EncodingVideo {
public:

    static void
    encode(std::shared_ptr<AVCodecContext> context, std::shared_ptr<AVFrame> frame, std::shared_ptr<AVPacket> packet,
           std::ofstream &stream);

public:
    EncodingVideo(const std::string &file) : stream(file) {
    }

    bool init();

    void loopEncode();

    void encode(std::vector<uint8_t> data, int width, int height, int pts);

    void flush();

private:
    std::ofstream stream;
    std::shared_ptr<AVCodecContext> codecContext;
    std::shared_ptr<AVCodec> codec;
    std::shared_ptr<AVPacket> packet;
    std::shared_ptr<AVFrame> frame;

};


#endif //CPP_ENCODEEXAMPLER_H
