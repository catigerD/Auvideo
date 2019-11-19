//
// Created by dengchong on 2019-10-14.
//

#include "EGLSharedContext.h"

#define LOG_TAG "EGLSharedContext"

using namespace std;

shared_ptr<EGLSharedContext> EGLSharedContext::instance = make_shared<EGLSharedContext>();

void EGLSharedContext::init() {
    sharedDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (EGL_NO_DISPLAY == sharedDisplay) {
        LOGE("eglGetDisplay() return error %d", eglGetError());
        return;
    }
    if (!eglInitialize(sharedDisplay, nullptr, nullptr)) {
        LOGE("eglInitialize() return error %d", eglGetError());
        return;
    }
    EGLint configAttrs[] = {
            EGL_BUFFER_SIZE, 32,
            EGL_RED_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_BLUE_SIZE, 8,
            EGL_ALPHA_SIZE, 8,
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
            EGL_NONE
    };
    EGLConfig config;
    EGLint numConfig;
    if (!eglChooseConfig(sharedDisplay, configAttrs, &config, 1, &numConfig)) {
        LOGE("eglChooseConfig() return error %d", eglGetError());
        return;
    }
    EGLint contextAttrs[] = {
            EGL_CONTEXT_CLIENT_VERSION, 2,
            EGL_NONE
    };
    if ((sharedContext = eglCreateContext(sharedDisplay, config,
                                          sharedContext == nullptr ? EGL_NO_CONTEXT : sharedContext,
                                          contextAttrs)) ==
        EGL_NO_CONTEXT) {
        LOGE("eglCreateContext() return error %d", eglGetError());
        return;
    }
}
