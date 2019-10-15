//
// Created by dengchong on 2019-09-26.
//

#include "Looper.h"
#include "Handler.h"//解决循环引用问题

#define LOG_TAG "Looper"

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

shared_ptr<Looper> Looper::prepare() {
    //需要注释这行，不然 threadLooper 对象只会初始化一次。
    //pthread_once(&allocLooperKeyOnce, alloc_pthread_key);
    return looper = make_shared<Looper>();
}

shared_ptr<Looper> Looper::getThreadLocalLooper() {
    return looper;
}

void Looper::loop() {
//    LOGI("Looper : ----------- %p", looper);
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
