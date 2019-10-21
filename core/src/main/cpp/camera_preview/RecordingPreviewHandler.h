//
// Created by dengchong on 2019-09-30.
//

#ifndef AUVIDEO_RECORDINGPREVIEWHANDLER_H
#define AUVIDEO_RECORDINGPREVIEWHANDLER_H

#include "Handler.h"
#include <memory>

using namespace std;

class RecordingPreviewController;

enum MSG_RECORDING_PREVIEW {
    MSG_INIT_EGL_CONTEXT = 0,
    MSG_UPDATE_TEX_IMAGE,
    MSG_RENDER_FRAME,
    MSG_DESTROY_EGL_CONTEXT,
    MSG_START_RECORDING,
    MSG_STOP_RECORDING,
};

class RecordingPreviewHandler : public Handler {
public:
    explicit RecordingPreviewHandler(shared_ptr<RecordingPreviewController> controller);

    void handleMessage(const shared_ptr<Message> &msg) override;

private:
    shared_ptr<RecordingPreviewController> controller;
};


#endif //AUVIDEO_RECORDINGPREVIEWHANDLER_H
