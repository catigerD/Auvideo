//
// Created by dengchong on 2019-09-25.
//

#ifndef AUVIDEO_EGLCORE_H
#define AUVIDEO_EGLCORE_H

#include <memory>

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>

#include "EGLSharedContext.h"
#include "AndroidLog.h"

class EGLCore {
public:
    EGLCore();

    ~EGLCore();

    bool init(EGLContext sharedContext = EGL_NO_CONTEXT);

    bool initWithSharedContext();

    EGLSurface createWindowSurface(std::shared_ptr<ANativeWindow> window);

    EGLSurface createOffscreenSurface(int width, int height);

    bool makeCurrent(EGLSurface surface);

    void doneCurrent();

    bool swapBuffers(EGLSurface surface);

    int querySurface(EGLSurface surface, int what);

    void releaseSurface(EGLSurface surface);

    void release();

    const EGLDisplay getDisplay() const {
        return display;
    }

    const EGLConfig getConfig() const {
        return config;
    }

    const EGLContext getContext() const {
        return context;
    }

private:
    EGLDisplay display{EGL_NO_DISPLAY};
    EGLConfig config{};
    EGLContext context{EGL_NO_CONTEXT};
};


#endif //AUVIDEO_EGLCORE_H
