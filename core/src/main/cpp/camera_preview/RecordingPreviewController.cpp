//
// Created by dengchong on 2019-09-30.
//

#include "RecordingPreviewController.h"

RecordingPreviewController::RecordingPreviewController()
        : eglCore(make_shared<EGLCore>()) {
    pthread_create(&startThreadTid, nullptr, startThread, this);
}

RecordingPreviewController::~RecordingPreviewController() {

}

void *RecordingPreviewController::startThread(void *context) {
    RecordingPreviewController *controller = static_cast<RecordingPreviewController *>(context);
    Looper::prepare();
    controller->handler = make_shared<Handler>();
    Looper::loop();
    pthread_exit(&controller->startThreadTid);
}

void RecordingPreviewController::sendInitEGLContextMsg(JavaVM *vm, jobject obj,
                                                       shared_ptr<ANativeWindow> window, int width,
                                                       int height) {
    while (handler) {
        this->javaVm = vm;
        this->obj = obj;
        this->window = window;
        surfaceWidth = width;
        surfaceHeight = height;
        handler->sendMessage(MSG_INIT_EGL_CONTEXT);
        return;
    }
}

void RecordingPreviewController::initEGLContext() {
    eglCore->init();
    surface = eglCore->createWindowSurface(window);
    eglCore->makeCurrent(surface);
    render = make_shared<RecordingPreviewRender>(surfaceWidth, surfaceHeight);
    configCameraToJava();
    setPreviewTextureToJava();
}

void RecordingPreviewController::configCameraToJava() {
    JNIEnv *env;
    if (javaVm->AttachCurrentThread(&env, nullptr) != JNI_OK) {
        return;
    }
    jclass schedulerClz = env->GetObjectClass(obj);
    jmethodID configCameraMid = env->GetMethodID(schedulerClz, "configCameraFromNative",
                                                 "()Lcom/dengchong/core/camera_preview/CameraInfo;");
    jobject cameraInfoObj = env->CallObjectMethod(obj, configCameraMid);
    jclass cameraInfoClz = env->GetObjectClass(cameraInfoObj);
    jmethodID getCameraWidthMid = env->GetMethodID(cameraInfoClz, "getCameraWidth", "()I");
    cameraWidth = env->CallIntMethod(cameraInfoObj, getCameraWidthMid);
    jmethodID getCameraHeightMid = env->GetMethodID(cameraInfoClz, "getCameraHeight", "()I");
    cameraHeight = env->CallIntMethod(cameraInfoObj, getCameraHeightMid);
    if (javaVm->DetachCurrentThread() != JNI_OK) {
        return;
    }
}

void RecordingPreviewController::setPreviewTextureToJava() {
    JNIEnv *env;
    if (javaVm->AttachCurrentThread(&env, nullptr) != JNI_OK) {
        return;
    }
    jclass schedulerClz = env->GetObjectClass(obj);
    jmethodID setPreviewTextureMid = env->GetMethodID(schedulerClz, "setPreviewTextureFromNative",
                                                      "(I)V");
    env->CallVoidMethod(obj, setPreviewTextureMid, render->getCameraTexId());
    if (javaVm->DetachCurrentThread() != JNI_OK) {
        return;
    }
}

void RecordingPreviewController::sendFrameAvailableMsg() {
    handler->sendMessage(MSG_UPDATE_TEX_IMAGE);
}

void RecordingPreviewController::updateTexImage() {
    //call java SurfaceTexture::updateTexImage();
    JNIEnv *env;
    if (javaVm->AttachCurrentThread(&env, nullptr) != JNI_OK) {
        return;
    }
    jclass schedulerClz = env->GetObjectClass(obj);
    jmethodID updateTexImageMid = env->GetMethodID(schedulerClz, "updateTexImageFromNative", "()V");
    env->CallVoidMethod(obj, updateTexImageMid);
    handler->sendMessage(MSG_RENDER_FRAME);
}

void RecordingPreviewController::renderFrame() {
    eglCore->makeCurrent(surface);
    render->processFrame();
    render->drawToView();
    eglCore->swapBuffers(surface);
}

void RecordingPreviewController::sendDestroyEGLContextMsg() {
    handler->sendMessage(MSG_DESTROY_EGL_CONTEXT);
}

void RecordingPreviewController::destroyEGLContext() {
    eglCore->releaseSurface(surface);
    eglCore->release();
}
