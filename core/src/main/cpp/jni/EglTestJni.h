//
// Created by dengchong on 2019-09-25.
//

#ifndef AUVIDEO_EGLTESTJNI_H
#define AUVIDEO_EGLTESTJNI_H


#include <jni.h>
#include "EglTest.h"
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include "GLSurfaceRenderTest.h"

shared_ptr<EglTest> eglTest;

static void releaseWindow(ANativeWindow *window) {
    ANativeWindow_release(window);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_dengchong_core_test_EGLTestMgr_prepareEglContext(JNIEnv *env, jclass clazz,
                                                          jobject surface, jint width,
                                                          jint height, jstring image_path) {
    shared_ptr<ANativeWindow> window = shared_ptr<ANativeWindow>(
            ANativeWindow_fromSurface(env, surface),
            releaseWindow);
    const char *imagePath = env->GetStringUTFChars(image_path, nullptr);
    eglTest = make_shared<GLSurfaceRenderTest>(window, width, height, imagePath);
    env->ReleaseStringUTFChars(image_path, imagePath);
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
    if (eglTest) {
        eglTest->destroyEglContext();
    }
}


#endif //AUVIDEO_EGLTESTJNI_H
