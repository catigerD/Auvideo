//
// Created by dengchong on 2019-10-16.
//

#include "VideoX264Encoder.h"

#include <thread>

#include "AndroidLog.h"

#define LOG_TAG "VideoX264Encoder"

using namespace std;
using namespace chrono;

VideoX264Encoder::VideoX264Encoder(string filePath, int width, int height, int bitRate,
                                   int frameRate)
        : fileName(std::move(filePath)),
//        stream(filePath),
          width(width),
          height(height),
          bitRate(bitRate),
          frameRate(frameRate) {
}

bool VideoX264Encoder::init() {
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
    codecContext->bit_rate = bitRate;
    codecContext->width = width;
    codecContext->height = height;
    codecContext->time_base = AVRational{1, frameRate};
    codecContext->framerate = AVRational{frameRate, 1};
    codecContext->gop_size = 10;
    codecContext->max_b_frames = 0;
    codecContext->pix_fmt = AV_PIX_FMT_YUV420P;
    if (codec->id == AV_CODEC_ID_H264) {
        //H.264
//        av_opt_set(codecContext->priv_data, "preset", "slow", 0);
        // 新增语句，设置为编码延迟
        av_opt_set(codecContext->priv_data, "preset", "ultrafast", 0);
        // 实时编码关键看这句，上面那条无所谓
        av_opt_set(codecContext->priv_data, "tune", "zerolatency", 0);
        av_opt_set(codecContext->priv_data, "profile", "main", 0);
    }
    muxer = make_shared<VideoX264Muxer>(fileName, codecContext);

    thread loopMuxingThread(&VideoX264Encoder::loopMuxing, this);
    loopMuxingThread.detach();

//    packet = FFmpegAlloc::getPacket();
//    if (!packet) {
//        LOGE("Could not allocate packet");
//        return false;
//    }
    frame = FFmpegAlloc::getFrame();
    if (!frame) {
        LOGE("Could not allocate frame");
        return false;
    }
    frame->format = codecContext->pix_fmt;
    frame->width = codecContext->width;
    frame->height = codecContext->height;
    int ret = av_frame_get_buffer(frame.get(), 16);
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

void VideoX264Encoder::encode(const shared_ptr<VideoFrame> &videoFrame) {
    LOGI("width : %d, height : %d", width, height);
    int ret = 0;
    ret = av_frame_make_writable(frame.get());
    if (ret < 0) {
        LOGE("av_frame_make_writable is false");
        return;
    }
    /* prepare a dummy image */
    /* Y */
    auto startTime = system_clock::now();
    memcpy(&frame->data[0][0], &videoFrame->data[0][0], videoFrame->lineSize[0] * height);
    memcpy(&frame->data[1][0], &videoFrame->data[1][0], videoFrame->lineSize[1] * height / 2);
    memcpy(&frame->data[2][0], &videoFrame->data[2][0], videoFrame->lineSize[2] * height / 2);
    auto copyDuration = duration_cast<milliseconds>(system_clock::now() - startTime);
    LOGI("encode : copyTime : %lld", copyDuration.count());
    int64_t pts = static_cast<int64_t>(videoFrame->timeMills.count() / av_q2d(codecContext->time_base) / 1000);
    frame->pts = pts;
    LOGI("encode pts : %lld， timeMills : %lld", pts, videoFrame->timeMills.count());
    /* encode the image */
    startTime = system_clock::now();
    encode(codecContext, frame/*, packet, stream*/);
    auto encodeTime = duration_cast<milliseconds>(system_clock::now() - startTime);
    LOGI("encode : encodeTime : %lld", encodeTime.count());
}

void VideoX264Encoder::flush() {
    LOGI("VideoX264Encoder::flush() start");
    encode(codecContext, nullptr/*, packet, stream*/);
    LOGI("VideoX264Encoder::flush() end");
//    stream.close();
    while (!packets.empty()) {
        this_thread::sleep_for(milliseconds(100));
    }
    packets.abort();
    LOGI("VideoX264Encoder::mux() end");
}

void
VideoX264Encoder::encode(shared_ptr<AVCodecContext> context, shared_ptr<AVFrame> frame
        /*,shared_ptr<AVPacket> packet,ofstream &stream*/) {
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
        shared_ptr<AVPacket> packet = FFmpegAlloc::getPacket();
        ret = avcodec_receive_packet(context.get(), packet.get());
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
            LOGE("Write wait ...");
            return;
        } else if (ret < 0) {
            LOGE("Error during encoding");
            return;
        }
        LOGI("Write packet , pts : %lld , packet size : %d", packet->pts, packet->size);
//        if (stream.good()) {
//            LOGI("Write before stream is good");
//        }
//        stream.write(reinterpret_cast<const char *>(packet->data), packet->size);
        packets.push(packet);
//        if (stream.good()) {
//            LOGI("Write after stream is good");
//        }
//        av_packet_unref(packet.get());
    }
}

void VideoX264Encoder::loopMuxing() {
    muxer->initAndWriteHeader();
    shared_ptr<AVPacket> packet{};
    while (true) {
        if (!packets.waitAndPop(packet)) {
            break;
        }
        muxer->writePacket(packet);
    }
    muxer->writeTrailer();
}
