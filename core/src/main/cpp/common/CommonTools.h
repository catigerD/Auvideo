//
// Created by dengchong on 2019-09-25.
//

#ifndef AUVIDEO_COMMONTOOLS_H
#define AUVIDEO_COMMONTOOLS_H

#include <android/log.h>
#include <pthread.h>

#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

class Lock {
public:
    Lock(pthread_mutex_t &mutex) : mutex(mutex) {
        pthread_mutex_lock(&mutex);
    }

    ~Lock() {
        pthread_mutex_unlock(&mutex);
    }

private:
    pthread_mutex_t &mutex;
};


#endif //AUVIDEO_COMMONTOOLS_H
