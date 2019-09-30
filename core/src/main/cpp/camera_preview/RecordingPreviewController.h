//
// Created by dengchong on 2019-09-30.
//

#ifndef AUVIDEO_RECORDINGPREVIEWCONTROLLER_H
#define AUVIDEO_RECORDINGPREVIEWCONTROLLER_H

#include "RecordingPreviewHandler.h"
#include "RecordingPreviewRender.h"
#include <pthread.h>
#include "Looper.h"
#include "EGLCore.h"
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include <jni.h>


class RecordingPreviewController {
    friend RecordingPreviewHandler;
public:
    RecordingPreviewController();

    ~RecordingPreviewController();

    void sendInitEGLContextMsg(JavaVM *vm, jobject obj, shared_ptr<ANativeWindow> window, int width,
                               int height);

    void sendFrameAvailableMsg();

    void resetSurfaceSize(int width, int height) {
        this->surfaceWidth = width;
        this->surfaceHeight = height;
    }

    void sendDestroyEGLContextMsg();

private:
    pthread_t startThreadTid{};

    static void *startThread(void *);

    shared_ptr<Handler> handler;
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

    int cameraWidth{};
    int cameraHeight{};

    void configCameraToJava();

    void setPreviewTextureToJava();
};


#endif //AUVIDEO_RECORDINGPREVIEWCONTROLLER_H
