//
// Created by dengchong on 2019-09-26.
//

#ifndef AUVIDEO_MESSAGE_H
#define AUVIDEO_MESSAGE_H

#include <memory>

class Handler;

class Message;

typedef void (*MessageCallback)(const std::shared_ptr<Message> &msg);

struct Message {
public:

    static std::shared_ptr<Message> obtain() {
        return std::make_shared<Message>();
    }

    static std::shared_ptr<Message> obtain(int what, std::shared_ptr<Handler> handler) {
        return std::make_shared<Message>(what, handler);
    }

public:
    Message() = default;

    Message(int what, std::shared_ptr<Handler> handler);

public:
    int what{};
    int arg1{};
    int arg2{};
    void *obj{};
    std::shared_ptr<Handler> handler;
    MessageCallback callback{};
};


#endif //AUVIDEO_MESSAGE_H
