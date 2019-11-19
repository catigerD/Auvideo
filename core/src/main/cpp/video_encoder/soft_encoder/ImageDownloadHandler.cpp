//
// Created by dengchong on 2019-10-14.
//

#include "ImageDownloadHandler.h"

#include "SoftEncoderAdapter.h"

using namespace std;

void ImageDownloadHandler::handleMessage(const shared_ptr<Message> &msg) {
    switch (msg->what) {
        case MSG_INIT_EGL_CONTEXT:
            adapter->initEglContext();
            break;
        case MSG_DOWNLOAD_TEXTURE:
            adapter->downloadTexture();
            break;
        case MSG_DESTROY_EGL_CONTEXT:
            adapter->destroyEglContext();
            break;
        default:
            break;
    }
}
