//
// Created by dengchong on 2019-09-26.
//

#include "Handler.h"

Handler::Handler() :
        looper(Looper::getThreadLocalLooper()) {

}

Handler::Handler(Looper *looper)
        : looper(looper) {

}

void Handler::sendMessage(shared_ptr<Message> &msg) {
    msg->handler = this;
    looper->enqueueMsg(msg);
}

void Handler::sendMessage(int what) {
    shared_ptr<Message> msg = Message::obtain(what, this);
    looper->enqueueMsg(msg);
}

void Handler::post(MessageCallback callback) {
    shared_ptr<Message> msg = Message::obtain();
    msg->callback = callback;
    msg->handler = this;
    looper->enqueueMsg(msg);
}
