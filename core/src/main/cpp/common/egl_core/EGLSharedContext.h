//
// Created by dengchong on 2019-10-14.
//

#ifndef AUVIDEO_EGLSHAREDCONTEXT_H
#define AUVIDEO_EGLSHAREDCONTEXT_H

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <CommonTools.h>
#include <memory>

using namespace std;

class EGLSharedContext {
public:
    ~EGLSharedContext() {

    }

    static EGLContext getSharedContext() {
        if (instance->sharedDisplay == EGL_NO_DISPLAY) {
            instance->init();
        }
        return instance->sharedContext;
    }

    EGLSharedContext() {
        init();
    }

private:
//    EGLSharedContext() {
//        init();
//    }

    static shared_ptr<EGLSharedContext> instance;

    EGLDisplay sharedDisplay;
    EGLContext sharedContext;

    void init();
};


#endif //AUVIDEO_EGLSHAREDCONTEXT_H
