//
// Created by dengchong on 2019-10-16.
//

#ifndef AUVIDEO_VIDEOX264ENCODER_H
#define AUVIDEO_VIDEOX264ENCODER_H

#include <string>
#include <fstream>

#include "VideoFrame.h"
#include "FFmpegAlloc.h"
#include "ThreadSafeQueue.h"
#include "VideoX264Muxer.h"

class VideoX264Encoder {
public:
    VideoX264Encoder(std::string filePath, int width, int height, int bitRate, int frameRate);

    ~VideoX264Encoder() = default;

    bool init();

    void encode(const std::shared_ptr<VideoFrame> &videoFrame);

    void flush();

private:
//    std::ofstream stream;
    const std::string fileName;
    int width;
    int height;
    int bitRate;
    int frameRate;

    std::shared_ptr<AVCodec> codec;
    std::shared_ptr<AVCodecContext> codecContext;
    std::shared_ptr<AVFrame> frame;
//    std::shared_ptr<AVPacket> packet;

    ThreadSafeQueue<std::shared_ptr<AVPacket>> packets;
    std::shared_ptr<VideoX264Muxer> muxer;

    void
    encode(std::shared_ptr<AVCodecContext> context, std::shared_ptr<AVFrame> frame
            /*, std::shared_ptr<AVPacket> packet,std::ofstream &stream*/);

    void loopMuxing();
};


#endif //AUVIDEO_VIDEOX264ENCODER_H
