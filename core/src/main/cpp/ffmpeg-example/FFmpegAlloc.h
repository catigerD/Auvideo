//
// Created by dengchong on 2019-10-16.
//

#ifndef AUVIDEO_FFMPEGALLOC_H
#define AUVIDEO_FFMPEGALLOC_H

#include <memory>
#include <string>

extern "C" {
#include "libavcodec/avcodec.h"
#include "libavutil/opt.h"
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"
#include "libavutil/parseutils.h"
};

using namespace std;

struct FFmpegAlloc {

    static shared_ptr<AVCodec> getCodecByName(const string &codeName) {
        return shared_ptr<AVCodec>(
                avcodec_find_encoder_by_name(codeName.c_str()), [](AVCodec *codec) {

                });
    }

    static shared_ptr<AVCodec> getCodecById(AVCodecID id) {
        return shared_ptr<AVCodec>(
                avcodec_find_encoder(id), [](AVCodec *codec) {

                }
        );
    }

    static shared_ptr<AVCodecContext> getCodecContext(shared_ptr<AVCodec> codec) {
        return shared_ptr<AVCodecContext>(
                avcodec_alloc_context3(codec.get()), [](AVCodecContext *codec) {
                    avcodec_free_context(&codec);
                });
    }

    static shared_ptr<AVPacket> getPacket() {
        return shared_ptr<AVPacket>(av_packet_alloc(), [](AVPacket *packet) {
            av_packet_free(&packet);
        });
    }

    static shared_ptr<AVFrame> getFrame() {
        return shared_ptr<AVFrame>(av_frame_alloc(), [](AVFrame *frame) {
            av_frame_free(&frame);
        });
    }

    static shared_ptr<SwsContext> getSwsContext(int srcW, int srcH, enum AVPixelFormat srcFormat,
                                                int dstW, int dstH, enum AVPixelFormat dstFormat) {
        return shared_ptr<SwsContext>(sws_getContext(srcW, srcH, srcFormat,
                                                     dstW, dstH, dstFormat,
                                                     SWS_BILINEAR, nullptr, nullptr, nullptr),
                                      [](SwsContext *swsContext) {
                                          sws_freeContext(swsContext);
                                      });
    }
};


#endif //AUVIDEO_FFMPEGALLOC_H
