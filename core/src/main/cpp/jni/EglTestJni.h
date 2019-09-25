//
// Created by dengchong on 2019-09-25.
//

#ifndef AUVIDEO_EGLTESTJNI_H
#define AUVIDEO_EGLTESTJNI_H


#include <jni.h>
#include "EglTest.h"
#include <android/native_window.h>
#include <android/native_window_jni.h>

shared_ptr<EglTest> eglTest;

static void releaseWindow(ANativeWindow *window) {
    ANativeWindow_release(window);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_dengchong_core_test_EGLTestMgr_prepareEglContext(JNIEnv *env, jclass clazz,
                                                          jobject surface, jint width,
                                                          jint height) {
    shared_ptr<ANativeWindow> window = shared_ptr<ANativeWindow>(
            ANativeWindow_fromSurface(env, surface),
            releaseWindow);
    eglTest = make_shared<EglTest>(window, width, height);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_dengchong_core_test_EGLTestMgr_resetRenderSize(JNIEnv *env, jclass clazz, jint width,
                                                        jint height) {
    if (eglTest) {
        eglTest->resetRenderSize(width, height);
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_dengchong_core_test_EGLTestMgr_destroyEglContext(JNIEnv *env, jclass clazz) {
    eglTest = nullptr;
}


#endif //AUVIDEO_EGLTESTJNI_H
