//
// Created by dengchong on 2019-10-14.
//

#ifndef AUVIDEO_SOFTENCODERADAPTER_H
#define AUVIDEO_SOFTENCODERADAPTER_H

#include "VideoEncoderAdapter.h"
#include <thread>
#include "ImageDownloadHandler.h"
#include "GLSurfaceRender.h"
#include "FBOTextureFrame.h"
#include "VideoFrame.h"
#include "ThreadSafeQueue.h"

using namespace chrono;

class SoftEncoderAdapter : public VideoEncoderAdapter {
public:
    SoftEncoderAdapter(const string &path, int width, int height, int bitRate, int frameRate);

    ~SoftEncoderAdapter();

    void createEncoder(shared_ptr<EGLCore> core, int inputTexId);

    void encode();

    void destroyEncoder();

private:
    void loopEncode();

    shared_ptr<ImageDownloadHandler> imageDownloadHandler;

    void loopImageDownload();

private:
    friend class ImageDownloadHandler;

    mutex downloadTextureLock;
    condition_variable downloadTextureCond;

    shared_ptr<EGLCore> sharedContext;
    int cameraTexId;
    shared_ptr<EGLCore> eglCore{make_shared<EGLCore>()};
    bool eglInit{false};
    EGLSurface offScreenSurface = EGL_NO_SURFACE;
    shared_ptr<GLSurfaceRender> render;
    shared_ptr<FBOTextureFrame> fboTextureFrame;

    void initEglContext();

    void downloadTexture();

    void destroyEglContext();

    time_point<system_clock> startTime;
    int encodedFrameCount{};

    //queue
    ThreadSafeQueue<shared_ptr<VideoFrame>> videoFrames;
};


#endif //AUVIDEO_SOFTENCODERADAPTER_H
