//
// Created by dengchong on 2019-09-30.
//

#ifndef AUVIDEO_RECORDINGPREVIEWJNI_H
#define AUVIDEO_RECORDINGPREVIEWJNI_H

#include <jni.h>
#include "RecordingPreviewController.h"
#include <memory>
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include <unistd.h>

using namespace std;

shared_ptr<RecordingPreviewController> controller;
shared_ptr<ANativeWindow> window;

extern "C"
JNIEXPORT void JNICALL
Java_com_dengchong_core_camera_1preview_RecordingPreviewScheduler_initEGLContext(JNIEnv *env,
                                                                                 jobject thiz,
                                                                                 jobject surface,
                                                                                 jint width,
                                                                                 jint height,
                                                                                 jint cameraId) {
    controller = make_shared<RecordingPreviewController>();
    window = shared_ptr<ANativeWindow>(ANativeWindow_fromSurface(env, surface),
                                       [](ANativeWindow *window) {
                                           ANativeWindow_release(window);
                                       });
    JavaVM *vm;
    env->GetJavaVM(&vm);
    jobject globalObj = env->NewGlobalRef(thiz);
    usleep(100 * 1000);
    controller->sendInitEGLContextMsg(vm, globalObj, window, width, height, cameraId);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_dengchong_core_camera_1preview_RecordingPreviewScheduler_notifyFrameAvailable(JNIEnv *env,
                                                                                       jobject thiz) {
    if (controller) {
        controller->sendFrameAvailableMsg();
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_dengchong_core_camera_1preview_RecordingPreviewScheduler_resetSurfaceSize(JNIEnv *env,
                                                                                   jobject thiz,
                                                                                   jint width,
                                                                                   jint height) {
    if (controller) {
        controller->resetSurfaceSize(width, height);
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_dengchong_core_camera_1preview_RecordingPreviewScheduler_destroySurface(JNIEnv *env,
                                                                                 jobject thiz) {
    if (controller) {
        controller->sendDestroyEGLContextMsg();
        controller = nullptr;
    }
}


#endif //AUVIDEO_RECORDINGPREVIEWJNI_H