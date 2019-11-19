//
// Created by dengchong on 2019-10-14.
//

#ifndef AUVIDEO_SOFTENCODERADAPTER_H
#define AUVIDEO_SOFTENCODERADAPTER_H

#include <thread>

#include "VideoEncoderAdapter.h"
#include "ImageDownloadHandler.h"
#include "GLSurfaceRender.h"
#include "FBOTextureFrame.h"
#include "VideoFrame.h"
#include "ThreadSafeQueue.h"
#include <R2YConverter.h>
#include "VideoX264Encoder.h"
#include "EncodingVideo.h"
#include "ImageTextureFrame.h"

class SoftEncoderAdapter : public VideoEncoderAdapter {
public:
    SoftEncoderAdapter(const std::string &path, int width, int height, int bitRate, int frameRate,
                       const std::string &waterPath);

    ~SoftEncoderAdapter() override;

    void createEncoder(std::shared_ptr<EGLCore> core, GLuint inputTexId) override;

    void encode() override;

    void destroyEncoder() override;

private:
    void loopEncode();

    std::shared_ptr<ImageDownloadHandler> imageDownloadHandler;

    void loopImageDownload();

private:
    friend class ImageDownloadHandler;

    std::mutex downloadTextureLock;
    std::condition_variable downloadTextureCond;

    std::shared_ptr<EGLCore> sharedContext;
    GLuint renderTexId;
    std::shared_ptr<EGLCore> eglCore{std::make_shared<EGLCore>()};
    bool eglInit{false};
    EGLSurface offScreenSurface = EGL_NO_SURFACE;
    std::shared_ptr<GLSurfaceRender> render;
    std::shared_ptr<FBOTextureFrame> fboTextureFrame;
    std::shared_ptr<ImageTextureFrame> waterTextureFrame;
    bool addWater{};

    void initEglContext();

    void downloadTexture();

    void destroyEglContext();

    std::chrono::time_point<std::chrono::system_clock> startTime{};
    int encodedFrameCount{};

    //queue
    ThreadSafeQueue<std::shared_ptr<VideoFrame>> videoFrames;
    std::shared_ptr<R2YConverter> converter;
    std::shared_ptr<VideoX264Encoder> encoder;

    bool saveRgbaOrYUVImage{true};
};


#endif //AUVIDEO_SOFTENCODERADAPTER_H
