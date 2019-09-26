//
// Created by dengchong on 2019-09-26.
//

#include "Looper.h"
#include "Handler.h"//解决循环引用问题

int Looper::enqueueMsg(const shared_ptr<Message> &msg) {
    return msgQueue.put(msg);
}

int Looper::dequeueMsg(shared_ptr<Message> &msg) {
    return msgQueue.get(msg, true);
}


/*--------------------------------static---------------------------------------*/

static pthread_key_t threadLooperKey;
static Looper *threadLooper = nullptr;
static pthread_once_t allocLooperKeyOnce = PTHREAD_ONCE_INIT;

static void destroy_pthread_key(void *arg) {
    delete threadLooper;
}

static void alloc_pthread_key() {
    pthread_key_create(&threadLooperKey, destroy_pthread_key);
    threadLooper = new Looper();
    pthread_setspecific(threadLooperKey, threadLooper);
}

Looper *Looper::prepare() {
    pthread_once(&allocLooperKeyOnce, alloc_pthread_key);
    return static_cast<Looper *>(pthread_getspecific(threadLooperKey));
}

Looper *Looper::getThreadLocalLooper() {
    return static_cast<Looper *>(pthread_getspecific(threadLooperKey));
}

void Looper::loop() {
    Looper *looper = getThreadLocalLooper();
    while (true) {
        shared_ptr<Message> msg;
        looper->dequeueMsg(msg);
        if (Message::MSG_QUIT_LOOP == msg->what) {
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
