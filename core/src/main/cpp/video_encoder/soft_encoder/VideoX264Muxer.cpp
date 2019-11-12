//
// Created by dengchong on 2019-11-09.
//

#include "VideoX264Muxer.h"
#include "AndroidLog.h"

using namespace std;

#define LOG_TAG "VideoX264Muxer"

/*------------------------------handler-------------------------------------*/


VideoX264Muxer::VideoX264Muxer(std::string fileName, std::shared_ptr<AVCodecContext> codecContext)
        : fileName(std::move(fileName)),
          codecContext(std::move(codecContext)) {
}

bool VideoX264Muxer::initAndWriteHeader() {
    LOGI("initAndWriteHeader start");
    int ret = 0;
    //1. alloc out format context
    formatContext = FFmpegAlloc::getOutputFormatContext(fileName);
    if (!formatContext) {
        LOGE("Could not deduce output format from file extension");
        return false;
    }
    if (formatContext->oformat->video_codec != AV_CODEC_ID_NONE) {
        if (!addStream()) {
            LOGE("Could not add Stream");
            return false;
        }
    }
    /* open the output file, if needed */
    if (!(formatContext->oformat->flags & AVFMT_NOFILE)) {
        ret = avio_open(&formatContext->pb, fileName.c_str(), AVIO_FLAG_WRITE);
        if (ret < 0) {
            LOGI("Could not open '%s': %s\n", fileName.c_str(), av_err2str(ret));
            return false;
        }
    }
    /* Write the stream header, if any. */
    ret = avformat_write_header(formatContext.get(), nullptr);
    if (ret < 0) {
        LOGI("Error occurred when opening output file: %s\n", av_err2str(ret));
        return false;
    }
    LOGI("initAndWriteHeader success");
    initSuc = true;
    return true;
}

void VideoX264Muxer::writePacket(const std::shared_ptr<AVPacket> &packet) {
    if (!initSuc) {
        return;
    }
    /* rescale output packet timestamp values from codec to stream timebase */
    av_packet_rescale_ts(packet.get(), codecContext->time_base, videoStream->time_base);
    LOGI("writePacket start : codecTimeBase -> %d,%d; streamTimeBase -> %d, %d",
         codecContext->time_base.num,
         codecContext->time_base.den, videoStream->time_base.num, videoStream->time_base.den);
    packet->stream_index = videoStream->id;
    /* Write the compressed frame to the media file. */
    log_packet(packet);
    auto ret = av_interleaved_write_frame(formatContext.get(), packet.get());
    if (ret < 0) {
        LOGI("Error while writing video frame: %s\n", av_err2str(ret));
    }
}

void VideoX264Muxer::writeTrailer() {
    LOGI("writeTrailer start");
    /* Write the trailer, if any. The trailer must be written before you
     * close the CodecContexts open when you wrote the header; otherwise
     * av_write_trailer() may try to use memory that was freed on
     * av_codec_close(). */
    av_write_trailer(formatContext.get());
    avio_closep(&formatContext->pb);
}

/*-------------------------------private-------------------------------------*/

bool VideoX264Muxer::addStream() {
    videoStream = FFmpegAlloc::getAVStream(formatContext);
    if (!videoStream) {
        LOGE("Could not allocate stream");
        return false;
    }
    videoStream->id = formatContext->nb_streams - 1;
//    videoStream->time_base = AVRational{1, 25};
    //todo 应该在编码前设置？
//    if (formatContext->oformat->flags & AVFMT_GLOBALHEADER) {
//        codecContext->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
//    }
    auto ret = avcodec_parameters_from_context(videoStream->codecpar, codecContext.get());
    if (ret < 0) {
        LOGI("Could not copy the stream parameters,code : %d, error : %s", ret, av_err2str(ret));
        return false;
    }
    return true;
}

void VideoX264Muxer::log_packet(const std::shared_ptr<AVPacket> &pkt) {
    AVRational *time_base = &formatContext->streams[pkt->stream_index]->time_base;
    LOGI("log_packet : pts:%s pts_time:%s "
         "dts:%s dts_time:%s "
         "duration:%s duration_time:%s "
         "stream_index:%d "
         "time_base->num : %d, time_base_den : %d ",
         av_ts2str(pkt->pts), av_ts2timestr(pkt->pts, time_base),
         av_ts2str(pkt->dts), av_ts2timestr(pkt->dts, time_base),
         av_ts2str(pkt->duration), av_ts2timestr(pkt->duration, time_base),
         time_base->num, time_base->den,
         pkt->stream_index);
}
