#ifndef __COUNTDOWNLATCH__
#define __COUNTDOWNLATCH__

#include <mutex>
#include <chrono>
#include <condition_variable>
#include <thread>

class CountDownLatch {
    volatile int m_rest;
    std::mutex mtx;
    std::condition_variable cv;

public:
    CountDownLatch(int n) : m_rest(n) {}

    void await();

    std::cv_status await(long time);

    void countDown();

    int getCount();

};

#endif
