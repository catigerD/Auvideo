//
// Created by dengchong on 2019-09-26.
//

#include "Looper.h"
#include "Handler.h"//解决循环引用问题

#define LOG_TAG "Looper"

using namespace std;

void Looper::enqueueMsg(const shared_ptr<Message> &msg) {
    msgQueue.push(msg);
}

bool Looper::dequeueMsg(shared_ptr<Message> &msg) {
    return msgQueue.waitAndPop(msg);
}

void Looper::quit() {
    msgQueue.abort();
}


/*--------------------------------static---------------------------------------*/

thread_local shared_ptr<Looper> looper{};

thread_local once_flag onceFlag;

shared_ptr<Looper> Looper::prepare() {
    call_once(onceFlag, []() {
        looper = make_shared<Looper>();
    });
    return looper;
}

shared_ptr<Looper> Looper::getThreadLocalLooper() {
    return looper;
}

void Looper::loop() {
    while (true) {
        shared_ptr<Message> msg;
        if (!looper->dequeueMsg(msg)) {
            return;
        }
        if (msg->callback) {
            msg->callback(msg);
        } else {
            if (msg->handler) {
                msg->handler->handleMessage(msg);
            }
        }
    }
}
