//
// Created by dengchong on 2019-10-16.
//

#ifndef AUVIDEO_VIDEOX264ENCODER_H
#define AUVIDEO_VIDEOX264ENCODER_H

#include "VideoFrame.h"
#include "FFmpegAlloc.h"
#include "AndroidLog.h"
#include <string>
#include <fstream>

using namespace std;

class VideoX264Encoder {
public:
    VideoX264Encoder(const string &filePath, int width, int height, int bitRate, int frameRate);

    ~VideoX264Encoder() = default;

    bool init();

    void encode(const shared_ptr<VideoFrame> &videoFrame);

    void flush();

private:
    ofstream stream;
    int width;
    int height;
    int bitRate;
    int frameRate;

    shared_ptr<AVCodec> codec;
    shared_ptr<AVCodecContext> codecContext;
    shared_ptr<AVFrame> frame;
    shared_ptr<AVPacket> packet;

    void encode(shared_ptr<AVCodecContext> context, shared_ptr<AVFrame> frame, shared_ptr<AVPacket> packet,
                ofstream &stream);
};


#endif //AUVIDEO_VIDEOX264ENCODER_H
