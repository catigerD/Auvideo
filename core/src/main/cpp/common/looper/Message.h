//
// Created by dengchong on 2019-09-26.
//

#ifndef AUVIDEO_MESSAGE_H
#define AUVIDEO_MESSAGE_H

#include <memory>

using namespace std;

class Handler;

class Message;

typedef void (*MessageCallback)(const shared_ptr<Message> &msg);

struct Message {
public:

    static const int MSG_QUIT_LOOP = INT_MAX;

    static shared_ptr<Message> obtain() {
        return make_shared<Message>();
    }

    static shared_ptr<Message> obtain(int what, Handler *handler) {
        return make_shared<Message>(what, handler);
    }

public:
    Message() = default;

    Message(int what, Handler *handler);

public:
    int what{};
    int arg1{};
    int arg2{};
    void *obj;
    Handler *handler;
    MessageCallback callback{};
};


#endif //AUVIDEO_MESSAGE_H
