#include <utility>

//
// Created by dengchong on 2019-09-26.
//

#include "Message.h"

Message::Message(int what, shared_ptr<Handler> handler)
        : what(what),
          handler(std::move(handler)) {

}
