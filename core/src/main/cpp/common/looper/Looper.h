//
// Created by dengchong on 2019-09-26.
//

#ifndef AUVIDEO_LOOPER_H
#define AUVIDEO_LOOPER_H

#include "CommonQueue.h"
#include "Message.h"

class Looper {

public:

    static Looper *prepare();

    static Looper *getThreadLocalLooper();

    static void loop();

public:

    Looper() = default;

    ~Looper() = default;

    int enqueueMsg(const shared_ptr<Message> &msg);

    int dequeueMsg(shared_ptr<Message> &msg);

    void quit();

private:
    CommonQueue<shared_ptr<Message>> msgQueue;
};


#endif //AUVIDEO_LOOPER_H
