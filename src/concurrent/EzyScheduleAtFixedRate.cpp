//
//  EzyScheduleAtFixedRate.cpp
//  ezyfox-server-cpp-client
//
//  Created by Dung Ta Van on 8/4/19.
//  Copyright © 2019 Young Monkeys. All rights reserved.
//

#include <chrono>
#include "EzyThread.h"
#include "EzyScheduleAtFixedRate.h"

EZY_NAMESPACE_START_WITH_ONLY(concurrent)

EzyScheduleAtFixedRate::EzyScheduleAtFixedRate(std::string threadName) {
    this->mActive = false;
    this->mThreadName = threadName;
}

void EzyScheduleAtFixedRate::schedule(std::function<void ()> task, int delay, int period) {
    std::unique_lock<std::mutex> lk(mMutex);
    if(mActive)
        return;
    this->mActive = true;
    auto func = [this] (std::function<void ()> t, int d, int p) {
        if(mThreadName.length() > 0)
            EzyThread::setCurrentThreadName(mThreadName);
        if(d > 0)
            std::this_thread::sleep_for(std::chrono::milliseconds(d));
        while(true) {
            if(stoppable())
                break;
            auto startTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            t();
            auto endTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            auto elapsedTime = (int)(endTime - startTime);
            int remainSleepTime = p - elapsedTime;
            if(remainSleepTime > 0)
                std::this_thread::sleep_for(std::chrono::milliseconds(remainSleepTime));
        }
        delete this;
    };
    this->mThread = std::thread(func, task, delay, period);
    this->mThread.detach();
}

void EzyScheduleAtFixedRate::stop() {
    std::unique_lock<std::mutex> lk(mMutex);
    this->mActive = false;
}

bool EzyScheduleAtFixedRate::stoppable() {
    std::unique_lock<std::mutex> lk(mMutex);
    return mActive == false;
}

EZY_NAMESPACE_END_WITH
