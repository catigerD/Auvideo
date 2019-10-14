//
// Created by dengchong on 2019-09-30.
//

#include <thread>
#include "RecordingPreviewController.h"

#define LOG_TAG "RecordingPreviewController"

RecordingPreviewController::RecordingPreviewController()
        : eglCore(make_shared<EGLCore>()) {
    std::thread thread(&RecordingPreviewController::startThread, this);
    thread.detach();
}

RecordingPreviewController::~RecordingPreviewController() {

}

void RecordingPreviewController::startThread() {
    std::unique_lock<std::mutex> uniqueLock(mutex);
    Looper::prepare();
    handler = make_shared<RecordingPreviewHandler>(this);
    uniqueLock.unlock();
    Looper::loop();
}

void RecordingPreviewController::sendInitEGLContextMsg(JavaVM *vm, jobject obj,
                                                       shared_ptr<ANativeWindow> window,
                                                       int surfaceWidth,
                                                       int surfaceHeight,
                                                       int cameraId) {
    std::lock_guard<std::mutex> lockGuard(mutex);
    this->javaVm = vm;
    this->obj = obj;
    this->window = window;
    this->surfaceWidth = surfaceWidth;
    this->surfaceHeight = surfaceHeight;
    this->cameraId = cameraId;
    handler->sendMessage(MSG_INIT_EGL_CONTEXT);
    LOGI("RecordingPreviewController::sendInitEGLContextMsg");
}

void RecordingPreviewController::initEGLContext() {
    LOGI("RecordingPreviewController::initEGLContext()");
    eglCore->init();
    surface = eglCore->createWindowSurface(window);
    eglCore->makeCurrent(surface);
    configCameraToJava();
    render = make_shared<RecordingPreviewRender>(surfaceWidth, surfaceHeight, cameraWidth,
                                                 cameraHeight, degress);
    render->init();
    setPreviewTextureToJava();
}

void RecordingPreviewController::configCameraToJava() {
    int status = 0;
    bool needAttach = false;
    JNIEnv *env;
    status = javaVm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_4);
    if (status < 0) {
        if (javaVm->AttachCurrentThread(&env, nullptr) != JNI_OK) {
            return;
        }
        needAttach = true;
    }
    jclass schedulerClz = env->GetObjectClass(obj);
    jmethodID configCameraMid = env->GetMethodID(schedulerClz, "configCameraFromNative",
                                                 "(I)Lcom/dengchong/core/camera_preview/CameraInfo;");
    jobject cameraInfoObj = env->CallObjectMethod(obj, configCameraMid, cameraId);
    jclass cameraInfoClz = env->GetObjectClass(cameraInfoObj);
    jmethodID getCameraWidthMid = env->GetMethodID(cameraInfoClz, "getCameraWidth", "()I");
    cameraWidth = env->CallIntMethod(cameraInfoObj, getCameraWidthMid);
    jmethodID getCameraHeightMid = env->GetMethodID(cameraInfoClz, "getCameraHeight", "()I");
    cameraHeight = env->CallIntMethod(cameraInfoObj, getCameraHeightMid);
    jmethodID getDegressMid = env->GetMethodID(cameraInfoClz, "getDegress", "()I");
    degress = env->CallIntMethod(cameraInfoObj, getDegressMid);
    if (needAttach) {
        if (javaVm->DetachCurrentThread() != JNI_OK) {
            return;
        }
    }
    LOGI("configCameraToJava");
}

void RecordingPreviewController::setPreviewTextureToJava() {
    int status = 0;
    bool needAttach = false;
    JNIEnv *env;
    status = javaVm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_4);
    if (status < 0) {
        if (javaVm->AttachCurrentThread(&env, nullptr) != JNI_OK) {
            return;
        }
        needAttach = true;
    }
    jclass schedulerClz = env->GetObjectClass(obj);
    jmethodID setPreviewTextureMid = env->GetMethodID(schedulerClz, "setPreviewTextureFromNative",
                                                      "(I)V");
    env->CallVoidMethod(obj, setPreviewTextureMid, render->getCameraTexId());
    if (needAttach) {
        if (javaVm->DetachCurrentThread() != JNI_OK) {
            return;
        }
    }
    LOGI("setPreviewTextureToJava");
}

void RecordingPreviewController::sendFrameAvailableMsg() {
    handler->sendMessage(MSG_UPDATE_TEX_IMAGE);
}

void RecordingPreviewController::updateTexImage() {
    //call java SurfaceTexture::updateTexImage();
    int status = 0;
    bool needAttach = false;
    JNIEnv *env;
    status = javaVm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_4);
    if (status < 0) {
        if (javaVm->AttachCurrentThread(&env, nullptr) != JNI_OK) {
            return;
        }
        needAttach = true;
    }
    jclass schedulerClz = env->GetObjectClass(obj);
    jmethodID updateTexImageMid = env->GetMethodID(schedulerClz, "updateTexImageFromNative", "()V");
    env->CallVoidMethod(obj, updateTexImageMid);
    handler->sendMessage(MSG_RENDER_FRAME);
    if (needAttach) {
        if (javaVm->DetachCurrentThread() != JNI_OK) {
            return;
        }
    }
    LOGI("updateTexImage");
}

void RecordingPreviewController::renderFrame() {
    eglCore->makeCurrent(surface);
    render->processFrame();
    render->drawToView();
    eglCore->swapBuffers(surface);
    LOGI("renderFrame");
}

void RecordingPreviewController::sendDestroyEGLContextMsg() {
    handler->sendMessage(MSG_DESTROY_EGL_CONTEXT);
}

void RecordingPreviewController::destroyEGLContext() {
    eglCore->releaseSurface(surface);
    surface = EGL_NO_SURFACE;
//    releaseCameraToJava();
    if (render) {
        render->destroy();
        render = nullptr;
    }
    eglCore->release();
    eglCore = nullptr;
    deleteGlobalObj();
    handler->getLooper()->quit();
}

void RecordingPreviewController::releaseCameraToJava() {
    int status = 0;
    bool needAttach = false;
    JNIEnv *env;
    status = javaVm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_4);
    if (status < 0) {
        if (javaVm->AttachCurrentThread(&env, nullptr) != JNI_OK) {
            return;
        }
        needAttach = true;
    }
    jclass schedulerClz = env->GetObjectClass(obj);
    jmethodID releaseCameraMid = env->GetMethodID(schedulerClz, "releaseCameraFromNative", "()V");
    env->CallVoidMethod(obj, releaseCameraMid);
    if (needAttach) {
        if (javaVm->DetachCurrentThread() != JNI_OK) {
            return;
        }
    }
    LOGI("releaseCameraToJava");
}

void RecordingPreviewController::deleteGlobalObj() {
    int status = 0;
    bool needAttach = false;
    JNIEnv *env;
    status = javaVm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_4);
    if (status < 0) {
        if (javaVm->AttachCurrentThread(&env, nullptr) != JNI_OK) {
            return;
        }
        needAttach = true;
    }
    if (obj) {
        env->DeleteGlobalRef(obj);
    }
    if (needAttach) {
        if (javaVm->DetachCurrentThread() != JNI_OK) {
            return;
        }
    }
    LOGI("deleteGlobalObj");
}
