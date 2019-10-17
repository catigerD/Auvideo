//
// Created by dengchong on 2019-10-16.
//

#include "VideoX264Encoder.h"

#define LOG_TAG "VideoX264Encoder"

VideoX264Encoder::VideoX264Encoder(const string &filePath, int width, int height, int bitRate,
                                   int frameRate)
        : stream(filePath),
          width(width),
          height(height),
          bitRate(bitRate),
          frameRate(frameRate) {
    init();
}

bool VideoX264Encoder::init() {
    codec = FfmpegAlloc::getCodecById(AV_CODEC_ID_H264);
    if (!codec) {
        LOGE("VideoX264Encoder::init() codec is not find");
        return false;
    }
    codecContext = FfmpegAlloc::getCodecContext(codec);
    if (!codecContext) {
        LOGE("VideoX264Encoder::init() could not alloc codec context");
        return false;
    }
    //设置 CodecContext 属性
    codecContext->bit_rate = bitRate;
    codecContext->width = width;
    codecContext->height = height;
    codecContext->time_base = AVRational{1, frameRate};
    codecContext->framerate = AVRational{frameRate, 1};
    codecContext->gop_size = frameRate;
    codecContext->max_b_frames = 0;
    codecContext->pix_fmt = AV_PIX_FMT_YUV420P;
    if (codec->id == AV_CODEC_ID_H264) {
        av_opt_set(codecContext->priv_data, "preset", "slow", 0);
    }
    //open it
    int ret = avcodec_open2(codecContext.get(), codec.get(), nullptr);
    if (ret < 0) {
        LOGE("VideoX264Encoder::init() avcodec_open2 error : %s", av_err2str(ret));
        return false;
    }

    frame = FfmpegAlloc::getFrame();
    if (!frame) {
        LOGE("VideoX264Encoder::init() could not alloc AVFrame");
        return false;
    }
    frame->format = codecContext->pix_fmt;
    frame->width = codecContext->width;
    frame->height = codecContext->height;

    packet = FfmpegAlloc::getPacket();
    if (!packet) {
        LOGE("VideoX264Encoder::init() could not alloc AVPacket");
        return false;
    }
    return true;
}

void VideoX264Encoder::encode(const shared_ptr<VideoFrame>& videoFrame) {
    int ret = av_frame_get_buffer(frame.get(), 0);
    if (ret < 0) {
        LOGE("VideoX264Encoder::encode could not alloc frame data");
        return;
    }
    ret = av_frame_make_writable(frame.get());
    if (ret < 0) {
        LOGE("VideoX264Encoder::av_frame_make_writable disable");
        return;
    }
    auto begin = videoFrame->data.begin();
    int step = videoFrame->data.size() / 6;
    copy(begin, begin + step * 4 - 1, frame->data[0]);
    copy(begin + step * 4, begin + step * 5 - 1, frame->data[1]);
    copy(begin + step * 5, videoFrame->data.end(), frame->data[2]);

    int64_t pts = static_cast<int64_t>(videoFrame->timeMills / 1000.0f /
                                       av_q2d(AVRational{1, 1000}));
    frame->pts = pts;
    //encode
    ret = avcodec_send_frame(codecContext.get(), frame.get());
    if (ret < 0) {
        LOGE("VideoX264Encoder::encode avcodec_send_frame error : %s", av_err2str(ret));
        return;
    }
    while (ret > 0) {
        ret = avcodec_receive_packet(codecContext.get(), packet.get());
        LOGI("VideoX264Encoder::avcodec_receive_packet");
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
            return;
        } else if (ret < 0) {
            LOGE("VideoX264Encoder::encode aavcodec_receive_packet error : %s", av_err2str(ret));
            return;
        }
        stream.write(reinterpret_cast<const char *>(packet->data), packet->size);
    }
}
