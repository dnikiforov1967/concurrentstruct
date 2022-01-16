#include "countdownlatch.h"

using namespace std::chrono_literals;

void CountDownLatch::await() {
    std::unique_lock<std::mutex> lck(mtx);
    while (m_rest > 0) {
        cv.wait(lck);
    }
}

std::cv_status CountDownLatch::await(long timeout) {
    std::unique_lock<std::mutex> lck(mtx);
    if (m_rest > 0) {
        return cv.wait_for(lck, timeout * 1ms);
    }
    else {
        return std::cv_status::no_timeout;
    }
}

void CountDownLatch::countDown() {
    std::unique_lock<std::mutex> lck(mtx);
    --m_rest;
    cv.notify_all();
}

int CountDownLatch::getCount() {
    std::unique_lock<std::mutex> lck(mtx);
    return m_rest;
}