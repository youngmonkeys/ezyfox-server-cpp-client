//
//  EzyScheduleAtFixedRate.cpp
//  ezyfox-server-cpp-client
//
//  Created by Dung Ta Van on 8/4/19.
//  Copyright © 2019 Young Monkeys. All rights reserved.
//

#include <thread>
#include <chrono>
#include "EzyThread.h"
#include "EzyScheduleAtFixedRate.h"
#include "../gc/EzyReleasePool.h"
#include "../gc/EzyAutoReleasePool.h"

EZY_NAMESPACE_START_WITH_ONLY(concurrent)

EzyScheduleAtFixedRate::EzyScheduleAtFixedRate(std::string threadName) {
    this->mActive = false;
    this->mThreadName = threadName;
}

EzyScheduleAtFixedRate::~EzyScheduleAtFixedRate() {
}

void EzyScheduleAtFixedRate::schedule(std::function<void ()> task, int delay, int period) {
    std::unique_lock<std::mutex> lk(mScheduleMutex);
    if(mActive)
        return;
    this->mActive = true;
    this->retain();
    std::thread newThread(&EzyScheduleAtFixedRate::startLoop, this, task, delay, period);
    newThread.detach();
}

void EzyScheduleAtFixedRate::startLoop(std::function<void ()> task, int delay, int period) {
    this->startLoop0(task, delay, period);
    this->release();
}

void EzyScheduleAtFixedRate::startLoop0(std::function<void ()> task, int delay, int period) {
    if(mThreadName.length() > 0)
        EzyThread::setCurrentThreadName(mThreadName);
    if(delay > 0)
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
#ifdef EZY_DEBUG
    auto releasePool = gc::EzyAutoReleasePool::getInstance()->newPool(mThreadName);
#else
    auto releasePool = gc::EzyAutoReleasePool::getInstance()->getPool();
#endif
    while(true) {
        releasePool->releaseAll();
        if(stoppable())
            break;
        auto startTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        task();
        auto endTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        auto elapsedTime = (int)(endTime - startTime);
        int remainSleepTime = period - elapsedTime;
        if(stoppable())
            break;
        if(remainSleepTime > 0)
            std::this_thread::sleep_for(std::chrono::milliseconds(remainSleepTime));
    }
    releasePool->releaseAll();
    gc::EzyAutoReleasePool::getInstance()->removePool();
}

void EzyScheduleAtFixedRate::stop() {
    std::unique_lock<std::mutex> lk(mScheduleMutex);
    this->mActive = false;
    this->release();
}

bool EzyScheduleAtFixedRate::stoppable() {
    std::unique_lock<std::mutex> lk(mScheduleMutex);
    return mActive == false;
}

EZY_NAMESPACE_END_WITH
