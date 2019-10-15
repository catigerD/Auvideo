//
// Created by dengchong on 2019-10-12.
//

#ifndef AUVIDEO_THREADSAFEQUEUE_H
#define AUVIDEO_THREADSAFEQUEUE_H

#include <memory>
#include <mutex>
#include <condition_variable>
#include <queue>

template<typename T>
class ThreadSafeQueue {
private:
    std::mutex mutex;
    std::condition_variable cond;
    std::queue<T> data;
    bool abortFlag{false};
public:
    ThreadSafeQueue() {

    }

    ThreadSafeQueue(const ThreadSafeQueue &other) {
        std::lock_guard<std::mutex> lockGuard(mutex);
        data = other.data;
    }

    ThreadSafeQueue &operator=(const ThreadSafeQueue &other) = delete;

    void push(T value) {
        std::lock_guard<std::mutex> lockGuard(mutex);
        data.push(value);
        cond.notify_one();
    }

    bool waitAndPop(T &value) {
        std::unique_lock<std::mutex> uniqueLock(mutex);
        cond.wait(uniqueLock, [this] { return !data.empty(); });
        if (abortFlag) {
            return false;
        }
        value = data.front();
        data.pop();
        return true;
    }

    std::shared_ptr<T> waitAndPop() {
        std::unique_lock<std::mutex> uniqueLock(mutex);
        cond.wait(uniqueLock, [this] { return !data.empty(); });
        if (abortFlag) {
            return nullptr;
        }
        std::shared_ptr<T> res(std::make_shared<T>(data.front()));
        data.pop();
        return res;
    }

    bool tryPop(T &value) {
        std::lock_guard<std::mutex> lockGuard(mutex);
        if (data.empty()) {
            return false;
        }
        value = data.front();
        data.pop();
        return true;
    }

    std::shared_ptr<T> tryPop() {
        std::lock_guard<std::mutex> lockGuard(mutex);
        if (data.empty()) {
            return nullptr;
        }
        std::shared_ptr<T> res(std::make_shared<T>(data.front()));
        data.pop();
        return res;
    }

    bool empty() const {
        std::lock_guard<std::mutex> lockGuard(mutex);
        return data.empty();
    }

    void abort() {
        std::lock_guard<std::mutex> lockGuard(mutex);
        abortFlag = true;
        cond.notify_all();
    }
};


#endif //AUVIDEO_THREADSAFEQUEUE_H
