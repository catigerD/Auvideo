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
#include <R2YConverter.h>
#include "VideoX264Encoder.h"

using namespace chrono;

class SoftEncoderAdapter : public VideoEncoderAdapter {
public:
    SoftEncoderAdapter(const string &path, int width, int height, int bitRate, int frameRate);

    ~SoftEncoderAdapter();

    void createEncoder(shared_ptr<EGLCore> core, GLuint inputTexId);

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
    GLuint cameraTexId;
    shared_ptr<EGLCore> eglCore{make_shared<EGLCore>()};
    bool eglInit{false};
    EGLSurface offScreenSurface = EGL_NO_SURFACE;
    shared_ptr<GLSurfaceRender> render;
    shared_ptr<FBOTextureFrame> fboTextureFrame;
    GLuint fbo;

    void initEglContext();

    void downloadTexture();

    void destroyEglContext();

    time_point<system_clock> startTime;
    int encodedFrameCount{};

    //queue
    ThreadSafeQueue<shared_ptr<VideoFrame>> videoFrames;
    shared_ptr<R2YConverter> converter;
    shared_ptr<VideoX264Encoder> encoder;
};


#endif //AUVIDEO_SOFTENCODERADAPTER_H
