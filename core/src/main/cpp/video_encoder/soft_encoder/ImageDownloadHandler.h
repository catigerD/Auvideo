//
// Created by dengchong on 2019-10-14.
//

#ifndef AUVIDEO_IMAGEDOWNLOADHANDLER_H
#define AUVIDEO_IMAGEDOWNLOADHANDLER_H

#include "Handler.h"

class SoftEncoderAdapter;

class ImageDownloadHandler : public Handler {
public:

    enum ImageDownloadMsg {
        MSG_INIT_EGL_CONTEXT = 0,
        MSG_DOWNLOAD_TEXTURE,
        MSG_DESTROY_EGL_CONTEXT
    };

    ImageDownloadHandler(SoftEncoderAdapter *adapter)
            : adapter(adapter) {

    }

    void handleMessage(const shared_ptr<Message> &msg);

private:
    SoftEncoderAdapter *adapter;
};


#endif //AUVIDEO_IMAGEDOWNLOADHANDLER_H
