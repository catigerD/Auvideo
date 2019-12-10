//
// Created by dengchong on 2019-09-30.
//

#ifndef AUVIDEO_RECORDPREVIEWHANDLER_H
#define AUVIDEO_RECORDPREVIEWHANDLER_H

#include "Handler.h"
#include <memory>

using namespace std;

class RecordPreviewController;

enum MSG_RECORDING_PREVIEW {
    MSG_INIT_EGL_CONTEXT = 0,
    MSG_UPDATE_TEX_IMAGE,
    MSG_RENDER_FRAME,
    MSG_DESTROY_EGL_CONTEXT,
    MSG_START_RECORDING,
    MSG_STOP_RECORDING,
    MSG_SWITCH_CAMERA,
};

class RecordPreviewHandler : public Handler {
public:
    explicit RecordPreviewHandler(shared_ptr<RecordPreviewController> controller);

    void handleMessage(const shared_ptr<Message> &msg) override;

private:
    shared_ptr<RecordPreviewController> controller;
};


#endif //AUVIDEO_RECORDPREVIEWHANDLER_H
