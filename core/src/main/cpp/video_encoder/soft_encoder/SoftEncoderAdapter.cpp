//
// Created by dengchong on 2019-10-14.
//

#include "SoftEncoderAdapter.h"

#define LOG_TAG "SoftEncoderAdapter"

SoftEncoderAdapter::SoftEncoderAdapter(const string &path, int width, int height, int bitRate,
                                       int frameRate)
        : VideoEncoderAdapter(path, width, height, bitRate, frameRate),
          render(make_shared<GLSurfaceRender>(width, height)),
          fboTextureFrame(make_shared<FBOTextureFrame>(width, height)),
          converter(make_shared<R2YConverter>(width, height)),
          encoder(make_shared<VideoX264Encoder>(path, width, height, bitRate, frameRate)) {

}

SoftEncoderAdapter::~SoftEncoderAdapter() {

}

void SoftEncoderAdapter::createEncoder(shared_ptr<EGLCore> core, GLuint inputTexId) {
    sharedContext = core;
    renderTexId = inputTexId;
    thread encoderThread(&SoftEncoderAdapter::loopEncode, this);
    encoderThread.detach();
    thread imageDownloadThread(&SoftEncoderAdapter::loopImageDownload, this);
    imageDownloadThread.detach();
    while (!imageDownloadHandler) {
        this_thread::sleep_for(microseconds(100));
    }
    imageDownloadHandler->sendMessage(ImageDownloadHandler::MSG_INIT_EGL_CONTEXT);
}

void SoftEncoderAdapter::encode() {
    while (!eglInit) {
        this_thread::sleep_for(microseconds(100));
    }
    LOGI("SoftEncoderAdapter::encode()");
    if (startTime.time_since_epoch().count() == 0) {
        startTime = system_clock::now();
    }
    auto encodingTime = duration_cast<milliseconds>(system_clock::now() - startTime);
    LOGI("encode , encodingTime : %lld ms", encodingTime.count());
    //need drop frames,时间单位为毫秒
    int expectedFrameCount = static_cast<int>(encodingTime.count() / 1000.0f * encodeFrameRate);
    if (expectedFrameCount < encodedFrameCount) {
        return;
    }
    LOGI("encode , expectedFrameCount : %d, encodedFrameCount : %d", expectedFrameCount,
         encodedFrameCount);
    encodedFrameCount++;
    unique_lock<mutex> downloadUniqueLock(downloadTextureLock);
    imageDownloadHandler->sendMessage(ImageDownloadHandler::MSG_DOWNLOAD_TEXTURE);
    downloadTextureCond.wait(downloadUniqueLock);
}

void SoftEncoderAdapter::destroyEncoder() {
    imageDownloadHandler->sendMessage(ImageDownloadHandler::MSG_DESTROY_EGL_CONTEXT);
}


/***********************************private*********************************************/

void SoftEncoderAdapter::loopEncode() {
    shared_ptr<VideoFrame> videoFrame;
    int count = 0;
    encoder->init();
    while (true) {
        if (!videoFrames.waitAndPop(videoFrame)) {
            LOGI("SoftEncoderAdapter::loopEncode() quit");
            break;
        }
        encoder->encode(videoFrame);

        LOGI("SoftEncoderAdapter::loopEncode() encode %d", count++);
//        //验证从GPU 下载的 rgba 或转换后的 YUV420P 没有问题
//        if (saveRgbaOrYUVImage) {
//            LOGI("SoftEncoderAdapter::saveImage()");
//            fstream stream(filePath);
//            stream.write(reinterpret_cast<const char *>(&videoFrame->data[0][0]), videoFrame->bufferSize);
//            saveRgbaOrYUVImage = false;
//        }
    }
    encoder->flush();
}

void SoftEncoderAdapter::loopImageDownload() {
    Looper::prepare();
    imageDownloadHandler = make_shared<ImageDownloadHandler>(this);
    Looper::loop();
}

void SoftEncoderAdapter::initEglContext() {
    eglInit = eglCore->init(sharedContext->getContext());
    offScreenSurface = eglCore->createOffscreenSurface(encodeWidth, encodeHeight);
    eglCore->makeCurrent(offScreenSurface);
    render->init();
    fboTextureFrame->initTexture();
}

void SoftEncoderAdapter::downloadTexture() {
    unique_lock<mutex> uniqueLock(downloadTextureLock);
    if (startTime.time_since_epoch().count() == 0) {
        return;
    }
    LOGI("downloadTexture start");
    auto recordingDuration = duration_cast<milliseconds>(system_clock::now() - startTime);
    LOGI("encode , recordingDuration : %lld ms", recordingDuration.count());
    eglCore->makeCurrent(offScreenSurface);
    //拷贝纹理到临时纹理
    glBindFramebuffer(GL_FRAMEBUFFER, fboTextureFrame->getFbo());
    render->renderToVFlipTexture(renderTexId, fboTextureFrame->getTexId());
    downloadTextureCond.notify_one();
    uniqueLock.unlock();
    //从显存 download 到内存
    uint8_t **outData;
    int *lineSize;
    int size = converter->convert(fboTextureFrame->getTexId(), &outData, &lineSize);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    shared_ptr<VideoFrame> videoFrame = make_shared<VideoFrame>();
    videoFrame->data = outData;
    videoFrame->lineSize = lineSize;
    videoFrame->bufferSize = size;
    videoFrame->timeMills = recordingDuration;
    videoFrames.push(videoFrame);
    LOGI("downloadTexture end");
}

void SoftEncoderAdapter::destroyEglContext() {
    eglCore->makeCurrent(offScreenSurface);
    fboTextureFrame->destroy();
    render->destroy();
    eglCore->releaseSurface(offScreenSurface);
    eglCore->release();
    imageDownloadHandler->getLooper()->quit();
    while (!videoFrames.empty()) {
        this_thread::sleep_for(milliseconds(60));
    }
    videoFrames.abort();
}
