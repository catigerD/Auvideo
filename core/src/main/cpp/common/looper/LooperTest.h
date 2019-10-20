//
// Created by dengchong on 2019-09-26.
//

#ifndef AUVIDEO_LOOPERTEST_H
#define AUVIDEO_LOOPERTEST_H

#include "Handler.h"
#include <memory>
#include "AndroidLog.h"
#include <thread>

using namespace std;

class LooperTestHandler : public Handler {

private:
    void handleMessage(const shared_ptr<Message> &msg);
};

class LooperTest {
public:
    static const int MSG_TEST = 100;

public:
    shared_ptr<Handler> handler;
    pthread_t testThread{};

    LooperTest();

    ~LooperTest();

private:

    void startTestThread();

    void loop();
};


#endif //AUVIDEO_LOOPERTEST_H
