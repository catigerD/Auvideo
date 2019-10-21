//
// Created by dengchong on 2019-09-30.
//

#ifndef AUVIDEO_RECORDINGPREVIEWCONTROLLER_H
#define AUVIDEO_RECORDINGPREVIEWCONTROLLER_H

#include "RecordingPreviewHandler.h"
#include "RecordingPreviewRender.h"
#include "Looper.h"
#include "EGLCore.h"
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include <jni.h>
#include <chrono>
#include "SoftEncoderAdapter.h"
#include <string>

using namespace chrono;

class RecordingPreviewController : public enable_shared_from_this<RecordingPreviewController> {
    friend RecordingPreviewHandler;
public:
    RecordingPreviewController();

    ~RecordingPreviewController();

    void sendInitEGLContextMsg(JavaVM *vm, jobject obj, shared_ptr<ANativeWindow> window,
                               int surfaceWidth,
                               int surfaceHeight,
                               int cameraId);

    void sendFrameAvailableMsg();

    void resetSurfaceSize(int surfaceWidth, int surfaceHeight) {
        this->surfaceWidth = surfaceWidth;
        this->surfaceHeight = surfaceHeight;
    }

    void sendDestroyEGLContextMsg();

    void sendStartEncodingMsg(const string &filePath, int width, int height, int bitRate,
                              int frameRate, bool hwEncoding);

    void sendStopEncodingMsg();

private:
    void startThread();

    shared_ptr<RecordingPreviewHandler> handler;
    shared_ptr<RecordingPreviewRender> render;

private:

    shared_ptr<EGLCore> eglCore;
    shared_ptr<ANativeWindow> window;
    int surfaceWidth{};
    int surfaceHeight{};
    EGLSurface surface;

    void initEGLContext();

    void updateTexImage();

    void renderFrame();

    void destroyEGLContext();

private:
    JavaVM *javaVm{};
    jobject obj{};

    int cameraId{};
    int cameraWidth{};
    int cameraHeight{};
    int degress{};

    void configCameraToJava();

    void setPreviewTextureToJava();

    void releaseCameraToJava();

    void deleteGlobalObj();

private:
    //encode
    shared_ptr<VideoEncoderAdapter> encoder;
    bool isEncoding{};

    void startRecording();

    void stopRecording();
};


#endif //AUVIDEO_RECORDINGPREVIEWCONTROLLER_H
