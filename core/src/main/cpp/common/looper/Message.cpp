//
// Created by dengchong on 2019-09-26.
//

#include "Message.h"

Message::Message(int what, Handler *handler)
        : what(what),
          handler(handler) {

}
