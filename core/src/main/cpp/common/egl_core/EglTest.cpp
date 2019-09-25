//
// Created by dengchong on 2019-09-25.
//

#include "EglTest.h"

EglTest::EglTest(shared_ptr<ANativeWindow> window, int width, int height)
        : window(window),
          width(width),
          height(height),
          msg(MSG_INIT_EGL_CONTEXT) {
    pthread_create(&loopThreadTid, nullptr, startLoopThread, this);
}

EglTest::~EglTest() {
    destroyEglContext();
};

void *EglTest::startLoopThread(void *context) {
    EglTest *test = static_cast<EglTest *>(context);
    test->startLoop();
    pthread_exit(&test->loopThreadTid);
}

void EglTest::startLoop() {
    bool enableLoop = true;
    while (enableLoop) {
        switch (msg) {
            case MSG_INIT_EGL_CONTEXT:
                initEglContext();
                break;
            case MSG_QUIT_LOOP:
                enableLoop = false;
                break;
            default:
                break;
        }
        msg = MSG_NONE;
        if (eglCore) {
            eglCore->makeCurrent(surface);
            drawRect();
            eglCore->swapBuffers(surface);
        }
    }
}

void EglTest::initEglContext() {
    eglCore = make_shared<EGLCore>();
    eglCore->init();
    surface = eglCore->createWindowSurface(window);
    eglCore->makeCurrent(surface);
}

void EglTest::resetRenderSize(int width, int height) {
    this->width = width;
    this->height = height;
}

void EglTest::drawRect() {
    glClearColor(1.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
}

void EglTest::destroyEglContext() {
    if (eglCore) {
        eglCore->releaseSurface(surface);
        eglCore->release();
    }
    eglCore = nullptr;
}
