//
// Created by dengchong on 2019-09-26.
//

#ifndef AUVIDEO_HANDLER_H
#define AUVIDEO_HANDLER_H

#include "Looper.h"


class Handler : public std::enable_shared_from_this<Handler> {

public:

    Handler();

    explicit Handler(shared_ptr<Looper> looper);

    virtual ~Handler() = default;

    virtual void handleMessage(const shared_ptr<Message> &msg) {}

    void sendMessage(shared_ptr<Message> &msg);

    void sendMessage(int what);

    void post(MessageCallback callback);

    shared_ptr<Looper> getLooper() {
        return looper;
    }

private:
    shared_ptr<Looper> looper;
};


#endif //AUVIDEO_HANDLER_H
