//
// Created by dengchong on 2019-09-30.
//

#include "RecordingPreviewHandler.h"
#include "RecordingPreviewController.h"

RecordingPreviewHandler::RecordingPreviewHandler(
        const shared_ptr<RecordingPreviewController> &controller)
        : controller(controller) {

}

void RecordingPreviewHandler::handleMessage(const shared_ptr<Message> &msg) {
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
        default:
            break;
    }
}
