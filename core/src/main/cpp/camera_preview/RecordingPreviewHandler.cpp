#include <utility>

//
// Created by dengchong on 2019-09-30.
//

#include "RecordingPreviewHandler.h"
#include "RecordingPreviewController.h"

#define LOG_TAG "RecordingPreviewHandler"

RecordingPreviewHandler::RecordingPreviewHandler(
        shared_ptr<RecordingPreviewController> controller)
        : controller(std::move(controller)) {

}

void RecordingPreviewHandler::handleMessage(const shared_ptr<Message> &msg) {
    LOGI("handleMessage  msg");
    switch (msg->what) {
        case MSG_INIT_EGL_CONTEXT:
            controller->initEGLContext();
            break;
        case MSG_UPDATE_TEX_IMAGE:
            controller->updateTexImage();
            break;
        case MSG_RENDER_FRAME:
            controller->renderFrame();
            break;
        case MSG_DESTROY_EGL_CONTEXT:
            controller->destroyEGLContext();
            break;
        case MSG_START_RECORDING:
            controller->startRecording();
            break;
        case MSG_STOP_RECORDING:
            controller->stopRecording();
            break;
        case MSG_SWITCH_CAMERA:
            controller->switchCamera();
            break;
        default:
            break;
    }
}
