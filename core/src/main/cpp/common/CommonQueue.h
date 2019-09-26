//
// Created by dengchong on 2019-09-26.
//

#ifndef AUVIDEO_COMMONQUEUE_H
#define AUVIDEO_COMMONQUEUE_H

#include <queue>
#include <pthread.h>
#include "CommonTools.h"

using namespace std;

template<class Element>
class CommonQueue {
public:

    CommonQueue();

    ~CommonQueue();

    int put(const Element &element);

    int get(Element &element, bool block);

    int size();

    void flush();

    void abort();

private:
    queue<Element> collection;
    pthread_mutex_t mutex{};
    pthread_cond_t cond{};
    bool abortRequest{};
};

template<class Element>
CommonQueue<Element>::CommonQueue() {
    pthread_mutex_init(&mutex, nullptr);
    pthread_cond_init(&cond, nullptr);
}

template<class Element>
CommonQueue<Element>::~CommonQueue() {
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
}

template<class Element>
int CommonQueue<Element>::put(const Element &element) {
    Lock lock(mutex);
    if (abortRequest) {
        return -1;
    }
    collection.push(element);
    pthread_cond_signal(&cond);
    return 0;
}

template<class Element>
int CommonQueue<Element>::get(Element &element, bool block) {
    Lock lock(mutex);
    while (true) {
        if (abortRequest) {
            return -1;
        }
        if (!collection.empty()) {
            element = collection.front();
            collection.pop();
            return 1;
        } else {
            if (block) {
                pthread_cond_wait(&cond, &mutex);
            } else {
                return 0;
            }
        }
    }
}

template<class Element>
int CommonQueue<Element>::size() {
    Lock lock(mutex);
    return collection.size();
}

template<class Element>
void CommonQueue<Element>::flush() {
    Lock lock(mutex);
    while (!collection.empty()) {
        collection.pop();
    }
    pthread_cond_signal(&cond);
}

template<class Element>
void CommonQueue<Element>::abort() {
    Lock lock(mutex);
    abortRequest = true;
}


#endif //AUVIDEO_COMMONQUEUE_H
