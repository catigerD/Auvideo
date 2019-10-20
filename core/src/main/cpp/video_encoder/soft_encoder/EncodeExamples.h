//
// Created by dengchong on 2019-10-18.
//

#ifndef CPP_ENCODEEXAMPLER_H
#define CPP_ENCODEEXAMPLER_H

#include "FfmpegAlloc.h"
#include <memory>
#include "AndroidLog.h"
#include <fstream>

using namespace std;

class EncodeExamples {
public:

    static void
    encode(shared_ptr<AVCodecContext> context, shared_ptr<AVFrame> frame, shared_ptr<AVPacket> packet,
           ofstream &stream);

public:
    EncodeExamples(const string &file) : stream(file) {

    }

    bool init();

    void loopEncode();

private:
    ofstream stream;
    shared_ptr<AVCodecContext> codecContext;
    shared_ptr<AVCodec> codec;
    shared_ptr<AVPacket> packet;
    shared_ptr<AVFrame> frame;

};


#endif //CPP_ENCODEEXAMPLER_H
