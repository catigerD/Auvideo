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
          converter(make_shared<R2YConverter>()),
          encoder(make_shared<VideoX264Encoder>(path, width, height, bitRate, frameRate)),
          encodeExamples(make_shared<EncodeExamples>(path)) {

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
//    encodeExamples->init();
    encoder->init();
    while (true) {
        if (!videoFrames.waitAndPop(videoFrame)) {
            break;
        }
        encoder->encode(videoFrame);
//        encodeExamples->encode(videoFrame->data, videoWidth, videoHeight, count);
        LOGI("SoftEncoderAdapter::loopEncode() encode %d", count++);
    }
//    encodeExamples->flush();
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
    eglCore->makeCurrent(offScreenSurface);
    //拷贝纹理到临时纹理
    glBindFramebuffer(GL_FRAMEBUFFER, fboTextureFrame->getFbo());
    render->renderToTexture(renderTexId, fboTextureFrame->getTexId());
    downloadTextureCond.notify_one();
    uniqueLock.unlock();
    //从显存 download 到内存
    int size = encodeWidth * encodeHeight * 3 / 2;
    vector<uint8_t> texData(size, 0);
    converter->convert(fboTextureFrame->getTexId(), encodeWidth, encodeHeight, texData);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    shared_ptr<VideoFrame> videoFrame = make_shared<VideoFrame>();
    videoFrame->data = texData;
    videoFrame->timeMills = recordingDuration;
    videoFrames.push(videoFrame);
    LOGI("downloadTexture end");
}

void SoftEncoderAdapter::destroyEglContext() {
    videoFrames.abort();
    eglCore->makeCurrent(offScreenSurface);
    fboTextureFrame->destroy();
    render->destroy();
    eglCore->releaseSurface(offScreenSurface);
    eglCore->release();
    imageDownloadHandler->getLooper()->quit();
}
