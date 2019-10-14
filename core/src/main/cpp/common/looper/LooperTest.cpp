//
// Created by dengchong on 2019-09-26.
//

#include "LooperTest.h"

#define LOG_TAG "LooperTest"

void LooperTestHandler::handleMessage(const shared_ptr<Message> &msg) {
    LOGI("LooperTestHandler::handleMessage --------------------\n");
}

LooperTest::~LooperTest() {
    LOGI("LooperTest::~LooperTest()");
}


LooperTest::LooperTest() {
    std::thread thread(&LooperTest::startTestThread, this);
    thread.detach();
}

void LooperTest::startTestThread() {
    loop();
}

void LooperTest::loop() {
    Looper::prepare();
    handler = make_shared<LooperTestHandler>();
    Looper::loop();
    LOGI("LooperTest child thread exit");
}
