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

extern "C" {
#include <libavformat/avformat.h>
};

#include "EncodeExamples.h"
#include "encode_video.h"
#include "ScalingVideo.h"

using namespace std;

shared_ptr<RecordingPreviewController> controller;
shared_ptr<ANativeWindow> window;
shared_ptr<EncodeExamples> encodeExamples;
shared_ptr<ScalingVideo> scaleVideo;

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
    controller->sendInitEGLContextMsg(vm, globalObj, window, width, height, cameraId);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_dengchong_core_camera_1preview_RecordingPreviewScheduler_notifyFrameAvailable(JNIEnv *env,
                                                                                       jobject thiz) {
    if (controller) {
        avformat_alloc_context();
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
        //send msg 是一个线程，destroy相关在子线程，这里置为 null有问题。
//        controller = nullptr;
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_dengchong_core_camera_1preview_RecordingPreviewScheduler_startRecording(JNIEnv *env,
                                                                                 jobject thiz,
                                                                                 jstring file_path,
                                                                                 jint width,
                                                                                 jint height,
                                                                                 jint bit_rate,
                                                                                 jint frame_rate,
                                                                                 jboolean hw_encoding) {
//    if (controller) {
//        auto path = env->GetStringUTFChars(file_path, nullptr);
//        controller->sendStartEncodingMsg(path, width, height, bit_rate, frame_rate,
//                                         hw_encoding);
//        env->ReleaseStringUTFChars(file_path, path);
//    }

//    auto path = env->GetStringUTFChars(file_path, nullptr);
//    encodeExamples = make_shared<EncodeExamples>(path);
//    encodeExamples->init();
//    encodeExamples->loopEncode();
//    encode(path);
//    env->ReleaseStringUTFChars(file_path, path);

    auto path = env->GetStringUTFChars(file_path, nullptr);
    scaleVideo = make_shared<ScalingVideo>(path);
    scaleVideo->convert("324x324");
    env->ReleaseStringUTFChars(file_path, path);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_dengchong_core_camera_1preview_RecordingPreviewScheduler_stopRecording(JNIEnv *env,
                                                                                jobject thiz) {
//    if (controller) {
//        controller->sendStopEncodingMsg();
//    }
}

#endif //AUVIDEO_RECORDINGPREVIEWJNI_H