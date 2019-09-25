//
// Created by dengchong on 2019-09-25.
//

#include "EglCore.h"

#define LOG_TAG "EglCore"

EglCore::EglCore() = default;

EglCore::~EglCore() {
    release();
}

bool EglCore::init(EGLContext sharedContext) {
    display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (EGL_NO_DISPLAY == display) {
        LOGE("eglGetDisplay() return error %d", eglGetError());
        return false;
    }
    if (!eglInitialize(display, nullptr, nullptr)) {
        LOGE("eglInitialize() return error %d", eglGetError());
        return false;
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
    EGLint numConfig;
    if (!eglChooseConfig(display, configAttrs, &config, 1, &numConfig)) {
        LOGE("eglChooseConfig() return error %d", eglGetError());
        release();
        return false;
    }
    EGLint contextAttrs[] = {
            EGL_CONTEXT_CLIENT_VERSION, 2,
            EGL_NONE
    };
    if (!eglCreateContext(display, config, sharedContext, configAttrs)) {
        LOGE("eglCreateContext() return error %d", eglGetError());
        release();
        return false;
    }
    return true;
}

bool EglCore::initWithSharedContext() {
    //todo
    return false;
}

EGLSurface EglCore::createWindowSurface(shared_ptr<ANativeWindow> window) {
    EGLSurface surface = EGL_NO_SURFACE;
    if (!window) {
        LOGE("EglCore::createWindowSurface() error , window is null");
        return surface;
    }
    EGLint format;
    if (!eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format)) {
        LOGE("eglGetConfigAttrib() return error %d", eglGetError());
        return surface;
    }
    ANativeWindow_setBuffersGeometry(window.get(), 0, 0, format);
    if ((surface = eglCreateWindowSurface(display, config, window.get(), nullptr)) ==
        EGL_NO_SURFACE) {
        LOGE("eglCreateWindowSurface() return error %d", eglGetError());
        return surface;
    }
    return surface;
}

EGLSurface EglCore::createOffscreenSurface(int width, int height) {
    EGLSurface surface = EGL_NO_SURFACE;
    EGLint pBufferAttrs[] = {
            EGL_WIDTH, width,
            EGL_HEIGHT, height,
            EGL_NONE
    };
    if ((surface = eglCreatePbufferSurface(display, config, pBufferAttrs)) == EGL_NO_SURFACE) {
        LOGE("eglCreatePbufferSurface() return error %d", eglGetError());
        return surface;
    }
    return surface;
}

bool EglCore::makeCurrent(EGLSurface surface) {
    return static_cast<bool>(eglMakeCurrent(display, surface, surface, context));
}

void EglCore::doneCurrent() {
    eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
}

bool EglCore::swapBuffers(EGLSurface surface) {
    return static_cast<bool>(eglSwapBuffers(display, surface));
}

int EglCore::querySurface(EGLSurface surface, int what) {
    EGLint value = -1;
    eglGetConfigAttrib(display, config, what, &value);
    return value;
}

void EglCore::releaseSurface(EGLSurface surface) {
    eglDestroySurface(display, surface);
    surface = EGL_NO_SURFACE;
}

void EglCore::release() {
    if (display != EGL_NO_DISPLAY && context != EGL_NO_CONTEXT) {
        doneCurrent();
        eglDestroyContext(display, context);
    }
    display = EGL_NO_DISPLAY;
    context = EGL_NO_CONTEXT;
}



