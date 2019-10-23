//
// Created by dengchong on 2019-10-18.
//

#include "EncodingVideo.h"

#define LOG_TAG "EncodeExamples"

void EncodingVideo::encode(shared_ptr<AVCodecContext> context, shared_ptr<AVFrame> frame, shared_ptr<AVPacket> packet,
                            ofstream &stream) {
    int ret;
    /*send the frame to the encoder*/
    if (frame) {
        LOGI("Send frame pts : %lld", frame->pts);
    }
    ret = avcodec_send_frame(context.get(), frame.get());
    if (ret < 0) {
        LOGE("Error sending a frame for encoding !!! err msg : %s", av_err2str(ret));
        return;
    }
    while (ret >= 0) {
        ret = avcodec_receive_packet(context.get(), packet.get());
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
            LOGE("Write wait ...");
            return;
        } else if (ret < 0) {
            LOGE("Error during encoding");
            return;
        }
        LOGI("Write packet , pts : %lld , packet size : %d", packet->pts, packet->size);
        if (stream.good()) {
            LOGI("Write before stream is good");
        }
        stream.write(reinterpret_cast<const char *>(packet->data), packet->size);
        if (stream.good()) {
            LOGI("Write after stream is good");
        }
        av_packet_unref(packet.get());
    }
}

bool EncodingVideo::init() {
    codec = FFmpegAlloc::getCodecById(AV_CODEC_ID_H264);
    if (!codec) {
        LOGE("Codec %s not find ... ", "h264");
        return false;
    }
    codecContext = FFmpegAlloc::getCodecContext(codec);
    if (!codecContext) {
        LOGE("Could not allocate video codec Context");
        return false;
    }
    codecContext->bit_rate = 400000;
    codecContext->width = 352;
    codecContext->height = 288;
    codecContext->time_base = AVRational{1, 25};
    codecContext->framerate = AVRational{25, 1};
    codecContext->gop_size = 10;
    codecContext->max_b_frames = 1;
    codecContext->pix_fmt = AV_PIX_FMT_YUV420P;
    if (codec->id == AV_CODEC_ID_H264) {
        av_opt_set(codecContext->priv_data, "preset", "slow", 0);
    }
    packet = FFmpegAlloc::getPacket();
    if (!packet) {
        LOGE("Could not allocate packet");
        return false;
    }
    frame = FFmpegAlloc::getFrame();
    if (!frame) {
        LOGE("Could not allocate frame");
        return false;
    }
    frame->format = codecContext->pix_fmt;
    frame->width = codecContext->width;
    frame->height = codecContext->height;
    int ret = av_frame_get_buffer(frame.get(), 32);
    if (ret < 0) {
        LOGE("Could not allocate frame data , %s", av_err2str(ret));
        return false;
    }

    ret = avcodec_open2(codecContext.get(), codec.get(), nullptr);
    if (ret < 0) {
        LOGE("Could not open codec : %s", av_err2str(ret));
        return false;
    }
    return true;
}

void EncodingVideo::loopEncode() {
    int ret = 0;
    /* encode 1 second of video */
    for (int i = 0; i < 25; i++) {
        /* make sure the frame data is writable */
        ret = av_frame_make_writable(frame.get());
        if (ret < 0) {
            LOGE("av_frame_make_writable is false");
            return;
        }
        /* prepare a dummy image */
        /* Y */
        for (int y = 0; y < codecContext->height; y++) {
            for (int x = 0; x < codecContext->width; x++) {
                frame->data[0][y * frame->linesize[0] + x] = x + y + i * 3;
            }
        }
        /* Cb and Cr */
        for (int y = 0; y < codecContext->height / 2; y++) {
            for (int x = 0; x < codecContext->width / 2; x++) {
                frame->data[1][y * frame->linesize[1] + x] = 128 + y + i * 2;
                frame->data[2][y * frame->linesize[2] + x] = 64 + x + i * 5;
            }
        }
        frame->pts = i;
        /* encode the image */
        encode(codecContext, frame, packet, stream);
    }
    /* flush the encoder */
    encode(codecContext, nullptr, packet, stream);
    flush();
    stream.close();
}

void EncodingVideo::encode(vector<uint8_t> data, int width, int height, int pts) {
    int ret = 0;
    ret = av_frame_make_writable(frame.get());
    if (ret < 0) {
        LOGE("av_frame_make_writable is false");
        return;
    }
    /* prepare a dummy image */
    /* Y */
    memcpy(data.data(), &frame->data[0][0], width * height);
    memcpy(data.data() + width * height, &frame->data[1][0], width * height / 4);
    memcpy(data.data() + width * height * 5 / 4, &frame->data[2][0], width * height / 4);
//    copy(data.begin(), data.begin() + width * height - 1, frame->data[0]);
//    copy(data.begin() + width * height, data.begin() + width * height + width * height / 4 - 1, frame->data[1]);
//    copy(data.begin() + width * height + width * height / 4, data.begin() + width * height + width * height / 2 - 1,
//         frame->data[2]);
    frame->pts = pts;
    /* encode the image */
    encode(codecContext, frame, packet, stream);
}

void EncodingVideo::flush() {
    encode(codecContext, nullptr, packet, stream);
    stream.close();
}
