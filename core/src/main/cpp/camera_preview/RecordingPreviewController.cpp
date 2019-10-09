//
// Created by dengchong on 2019-09-30.
//

#include "RecordingPreviewController.h"

#define LOG_TAG "RecordingPreviewController"

RecordingPreviewController::RecordingPreviewController()
        : eglCore(make_shared<EGLCore>()) {
    pthread_mutex_init(&mutex, nullptr);
    pthread_create(&startThreadTid, nullptr, startThread, this);
}

RecordingPreviewController::~RecordingPreviewController() {

}

void *RecordingPreviewController::startThread(void *context) {
    RecordingPreviewController *controller = static_cast<RecordingPreviewController *>(context);
    pthread_mutex_lock(&controller->mutex);
    Looper::prepare();
    controller->handler = make_shared<RecordingPreviewHandler>(controller);
    pthread_mutex_unlock(&controller->mutex);
    Looper::loop();
    pthread_exit(&controller->startThreadTid);
}

void RecordingPreviewController::sendInitEGLContextMsg(JavaVM *vm, jobject obj,
                                                       shared_ptr<ANativeWindow> window,
                                                       int surfaceWidth,
                                                       int surfaceHeight) {
    pthread_mutex_lock(&mutex);
    this->javaVm = vm;
    this->obj = obj;
    this->window = window;
    surfaceWidth = surfaceWidth;
    surfaceHeight = surfaceHeight;
    handler->sendMessage(MSG_INIT_EGL_CONTEXT);
    LOGI("RecordingPreviewController::sendInitEGLContextMsg");
    pthread_mutex_unlock(&mutex);
}

void RecordingPreviewController::initEGLContext() {
    LOGI("RecordingPreviewController::initEGLContext()");
    eglCore->init();
    surface = eglCore->createWindowSurface(window);
    eglCore->makeCurrent(surface);
    configCameraToJava();
    render = make_shared<RecordingPreviewRender>(surfaceWidth, surfaceHeight, cameraWidth,
                                                 cameraHeight);
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
                                                 "()Lcom/dengchong/core/camera_preview/CameraInfo;");
    jobject cameraInfoObj = env->CallObjectMethod(obj, configCameraMid);
    jclass cameraInfoClz = env->GetObjectClass(cameraInfoObj);
    jmethodID getCameraWidthMid = env->GetMethodID(cameraInfoClz, "getCameraWidth", "()I");
    cameraWidth = env->CallIntMethod(cameraInfoObj, getCameraWidthMid);
    jmethodID getCameraHeightMid = env->GetMethodID(cameraInfoClz, "getCameraHeight", "()I");
    cameraHeight = env->CallIntMethod(cameraInfoObj, getCameraHeightMid);
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
    releaseCameraToJava();
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
