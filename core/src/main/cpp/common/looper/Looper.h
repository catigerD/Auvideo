//
// Created by dengchong on 2019-09-26.
//

#ifndef AUVIDEO_LOOPER_H
#define AUVIDEO_LOOPER_H

#include "ThreadSafeQueue.h"
#include "Message.h"

class Looper {

public:
    static const int MSG_QUIT_LOOP = -11111;

    static shared_ptr<Looper> prepare();

    static shared_ptr<Looper> getThreadLocalLooper();

    static void loop();

public:

    Looper() = default;

    ~Looper() = default;

    void enqueueMsg(const shared_ptr<Message> &msg);

    void dequeueMsg(shared_ptr<Message> &msg);

    void quit();

private:
    ThreadSafeQueue<shared_ptr<Message>> msgQueue;
};


#endif //AUVIDEO_LOOPER_H
