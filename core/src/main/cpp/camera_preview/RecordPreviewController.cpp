//
// Created by dengchong on 2019-09-30.
//

#include <thread>
#include "RecordPreviewController.h"

#define LOG_TAG "RecordingPreviewController"

RecordPreviewController::RecordPreviewController()
        : eglCore(make_shared<EGLCore>()) {
    std::thread thread(&RecordPreviewController::startThread, this);
    thread.detach();
}

RecordPreviewController::~RecordPreviewController() = default;

void RecordPreviewController::startThread() {
    Looper::prepare();
    handler = make_shared<RecordPreviewHandler>(shared_from_this());
    Looper::loop();
}

void RecordPreviewController::sendInitEGLContextMsg(JavaVM *vm, jobject obj,
                                                       shared_ptr<ANativeWindow> window,
                                                       int surfaceWidth,
                                                       int surfaceHeight,
                                                       int cameraId) {
    while (!handler) {
        this_thread::sleep_for(microseconds(100));
    }
    this->javaVm = vm;
    this->obj = obj;
    this->window = window;
    this->viewWidth = surfaceWidth;
    this->viewHeight = surfaceHeight;
    this->cameraId = cameraId;
    handler->sendMessage(MSG_INIT_EGL_CONTEXT);
    LOGI("RecordingPreviewController::sendInitEGLContextMsg");
}

void RecordPreviewController::initEGLContext() {
    LOGI("RecordingPreviewController::initEGLContext()");
    eglCore->init();
    surface = eglCore->createWindowSurface(window);
    eglCore->makeCurrent(surface);
    configCameraToJava();
    render = make_shared<RecordPreviewRender>(viewWidth, viewHeight, texWidth,
                                                 texHeight, degress, cameraId == CAMERA_FACING_FRONT);
    render->init();
    setPreviewTextureToJava();
}

void RecordPreviewController::configCameraToJava() {
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
    jmethodID getCameraWidthMid = env->GetMethodID(cameraInfoClz, "getPreviewWidth", "()I");
    texWidth = env->CallIntMethod(cameraInfoObj, getCameraWidthMid);
    jmethodID getCameraHeightMid = env->GetMethodID(cameraInfoClz, "getPreviewHeight", "()I");
    texHeight = env->CallIntMethod(cameraInfoObj, getCameraHeightMid);
    jmethodID getDegressMid = env->GetMethodID(cameraInfoClz, "getDegress", "()I");
    degress = env->CallIntMethod(cameraInfoObj, getDegressMid);
    if (needAttach) {
        if (javaVm->DetachCurrentThread() != JNI_OK) {
            return;
        }
    }
    LOGI("configCameraToJava --- previewWidth : %d, previewHeight : %d, degress : %d, surfaceWidth : %d, surfaceHeight : "
         "%d ... ", texWidth, texHeight, degress, viewWidth, viewHeight);
}

void RecordPreviewController::setPreviewTextureToJava() {
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

void RecordPreviewController::sendFrameAvailableMsg() {
    if (isSwitching) {
        return;
    }
    handler->sendMessage(MSG_UPDATE_TEX_IMAGE);
}

void RecordPreviewController::updateTexImage() {
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

void RecordPreviewController::renderFrame() {
    eglCore->makeCurrent(surface);
    render->processFrame();
    render->drawToView();
    eglCore->swapBuffers(surface);
    LOGI("renderFrame");
    if (isEncoding) {
        encoder->encode();
    }
}

void RecordPreviewController::sendDestroyEGLContextMsg() {
    handler->sendMessage(MSG_DESTROY_EGL_CONTEXT);
}

void RecordPreviewController::destroyEGLContext() {
    eglCore->makeCurrent(surface);
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

void RecordPreviewController::releaseCameraToJava() {
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

void RecordPreviewController::deleteGlobalObj() {
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

/*--------------------------encode----------------------------------*/

void RecordPreviewController::sendStartEncodingMsg(const string &filePath, int width, int height,
                                                      int bitRate, int frameRate, bool hwEncoding,
                                                      const string &waterPath) {
    if (!hwEncoding) {
        encoder = make_shared<SoftEncoderAdapter>(filePath, width, height, bitRate, frameRate, waterPath);
    }
    if (handler) {
        handler->sendMessage(MSG_START_RECORDING);
    }
}

void RecordPreviewController::sendStopEncodingMsg() {
    if (handler) {
        handler->sendMessage(MSG_STOP_RECORDING);
    }
}

void RecordPreviewController::startRecording() {
    encoder->createEncoder(eglCore, render->getRenderTexId());
    isEncoding = true;
}

void RecordPreviewController::stopRecording() {
    encoder->destroyEncoder();
    isEncoding = false;
    //todo encoder = nullptr,会导致encoder 对象被析构但子线程还在运行导致的空指针异常。
//    encoder = nullptr;
}

/*--------------------------switch camera----------------------------------*/

void RecordPreviewController::sendSwitchCameraMsg() {
    if (cameraId == CAMERA_FACING_BACK) {
        cameraId = CAMERA_FACING_FRONT;
    } else {
        cameraId = CAMERA_FACING_BACK;
    }
    handler->sendMessage(MSG_SWITCH_CAMERA);
}

void RecordPreviewController::switchCamera() {
    isSwitching = true;
    releaseCameraToJava();
    configCameraToJava();
    render->setDegress(degress, cameraId == CAMERA_FACING_FRONT);
    setPreviewTextureToJava();
    isSwitching = false;
}
