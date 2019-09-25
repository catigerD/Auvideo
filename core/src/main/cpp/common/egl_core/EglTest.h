//
// Created by dengchong on 2019-09-25.
//

#ifndef AUVIDEO_EGLTEST_H
#define AUVIDEO_EGLTEST_H

#include "EGLCore.h"
#include <pthread.h>
#include "GLTools.h"
#include <memory>

using namespace std;

class EglTest {
public:
    EglTest(shared_ptr<ANativeWindow> window, int width, int height);

    virtual ~EglTest();

    void resetRenderSize(int width, int height);

protected:

    void initEglContext();

    virtual void drawRect();

    void destroyEglContext();

protected:
    shared_ptr<EGLCore> eglCore;
    EGLSurface surface;

    shared_ptr<ANativeWindow> window;
    int width{};
    int height{};

    pthread_t loopThreadTid;

    static void *startLoopThread(void *context);

    void startLoop();

    enum EglTestMsg {
        MSG_NONE = 0, MSG_INIT_EGL_CONTEXT, MSG_QUIT_LOOP
    };
    EglTestMsg msg{MSG_NONE};
};


#endif //AUVIDEO_EGLTEST_H
