//
// Created by dengchong on 2019-09-26.
//

#ifndef AUVIDEO_HANDLER_H
#define AUVIDEO_HANDLER_H

#include "Looper.h"


class Handler {

public:

    Handler();

    Handler(Looper *looper);

    virtual ~Handler() = default;

    virtual void handleMessage(const shared_ptr<Message> &msg) {}

    void sendMessage(shared_ptr<Message> &msg);

    void sendMessage(int what);

    void post(MessageCallback callback);

private:
    Looper *looper;
};


#endif //AUVIDEO_HANDLER_H
