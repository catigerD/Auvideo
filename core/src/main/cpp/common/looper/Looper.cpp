//
// Created by dengchong on 2019-09-26.
//

#include "Looper.h"
#include "Handler.h"//解决循环引用问题

#define LOG_TAG "Looper"

int Looper::enqueueMsg(const shared_ptr<Message> &msg) {
    return msgQueue.put(msg);
}

int Looper::dequeueMsg(shared_ptr<Message> &msg) {
    return msgQueue.get(msg, true);
}

void Looper::quit() {
    msgQueue.abort();
}


/*--------------------------------static---------------------------------------*/

static pthread_key_t threadLooperKey;
static Looper *threadLooper = nullptr;
static pthread_once_t allocLooperKeyOnce = PTHREAD_ONCE_INIT;

static void destroy_pthread_key(void *arg) {
//    LOGI("Looper : destroy----------- %p", threadLooper);
    delete threadLooper;
}

static void alloc_pthread_key() {
    pthread_key_create(&threadLooperKey, destroy_pthread_key);
    threadLooper = new Looper();
    pthread_setspecific(threadLooperKey, threadLooper);
}

Looper *Looper::prepare() {
    //需要注释这行，不然 threadLooper 对象只会初始化一次。
    //pthread_once(&allocLooperKeyOnce, alloc_pthread_key);
    alloc_pthread_key();
    return static_cast<Looper *>(pthread_getspecific(threadLooperKey));
}

Looper *Looper::getThreadLocalLooper() {
    return static_cast<Looper *>(pthread_getspecific(threadLooperKey));
}

void Looper::loop() {
    Looper *looper = getThreadLocalLooper();
//    LOGI("Looper : ----------- %p", looper);
    while (true) {
        shared_ptr<Message> msg;
        if (looper->dequeueMsg(msg) < 0) {
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
