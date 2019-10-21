#include <utility>

//
// Created by dengchong on 2019-09-26.
//

#include "Handler.h"

Handler::Handler() :
        looper(Looper::getThreadLocalLooper()) {

}

Handler::Handler(shared_ptr<Looper> looper)
        : looper(std::move(looper)) {

}

void Handler::sendMessage(shared_ptr<Message> &msg) {
    msg->handler = shared_from_this();
    looper->enqueueMsg(msg);
}

void Handler::sendMessage(int what) {
    shared_ptr<Message> msg = Message::obtain(what, shared_from_this());
    looper->enqueueMsg(msg);
}

void Handler::post(MessageCallback callback) {
    shared_ptr<Message> msg = Message::obtain();
    msg->callback = callback;
    msg->handler = shared_from_this();
    looper->enqueueMsg(msg);
}
