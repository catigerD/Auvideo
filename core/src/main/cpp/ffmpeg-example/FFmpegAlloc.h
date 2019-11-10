//
// Created by dengchong on 2019-10-16.
//

#ifndef AUVIDEO_FFMPEGALLOC_H
#define AUVIDEO_FFMPEGALLOC_H

#include <memory>
#include <string>

extern "C" {
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libavutil/opt.h"
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"
#include "libavutil/parseutils.h"
};

struct FFmpegAlloc {

    static std::shared_ptr<AVCodec> getCodecByName(const std::string &codeName) {
        return std::shared_ptr<AVCodec>(
                avcodec_find_encoder_by_name(codeName.c_str()), [](AVCodec *codec) {

                });
    }

    static std::shared_ptr<AVCodec> getCodecById(AVCodecID id) {
        return std::shared_ptr<AVCodec>(
                avcodec_find_encoder(id), [](AVCodec *codec) {

                }
        );
    }

    static std::shared_ptr<AVCodecContext> getCodecContext(std::shared_ptr<AVCodec> codec) {
        return std::shared_ptr<AVCodecContext>(
                avcodec_alloc_context3(codec.get()), [](AVCodecContext *codec) {
                    avcodec_close(codec);
                    avcodec_free_context(&codec);
                });
    }

    static std::shared_ptr<AVPacket> getPacket() {
        return std::shared_ptr<AVPacket>(av_packet_alloc(), [](AVPacket *packet) {
            av_packet_free(&packet);
        });
    }

    static std::shared_ptr<AVFrame> getFrame() {
        return std::shared_ptr<AVFrame>(av_frame_alloc(), [](AVFrame *frame) {
            av_frame_free(&frame);
        });
    }

    static std::shared_ptr<SwsContext> getSwsContext(int srcW, int srcH, enum AVPixelFormat srcFormat,
                                                     int dstW, int dstH, enum AVPixelFormat dstFormat) {
        return std::shared_ptr<SwsContext>(sws_getContext(srcW, srcH, srcFormat,
                                                          dstW, dstH, dstFormat,
                                                          SWS_BILINEAR, nullptr, nullptr, nullptr),
                                           [](SwsContext *swsContext) {
                                               sws_freeContext(swsContext);
                                           });
    }

    static std::shared_ptr<AVFormatContext> getOutputFormatContext(const std::string &fileName) {
        AVFormatContext *formatContext;
        avformat_alloc_output_context2(&formatContext, nullptr, nullptr, fileName.c_str());
        return std::shared_ptr<AVFormatContext>(formatContext, [](AVFormatContext *context) {
            avformat_free_context(context);
        });
    }

    static std::shared_ptr<AVStream> getAVStream(std::shared_ptr<AVFormatContext> formatContext) {
        return std::shared_ptr<AVStream>(avformat_new_stream(formatContext.get(), nullptr), [](AVStream *stream) {

        });
    }
};


#endif //AUVIDEO_FFMPEGALLOC_H
