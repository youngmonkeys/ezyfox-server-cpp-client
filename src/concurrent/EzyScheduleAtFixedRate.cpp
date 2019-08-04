//
//  EzyScheduleAtFixedRate.cpp
//  ezyfox-server-cpp-client
//
//  Created by Dung Ta Van on 8/4/19.
//  Copyright © 2019 Young Monkeys. All rights reserved.
//

#include <chrono>
#include "EzyScheduleAtFixedRate.h"

EZY_NAMESPACE_START_WITH_ONLY(concurrent)

EzyScheduleAtFixedRate::EzyScheduleAtFixedRate() {
    mActive = false;
    mStarted = false;
}

void EzyScheduleAtFixedRate::schedule(std::function<void ()> task, int delay, int period) {
    if(mStarted)
        return;
    this->mStarted = true;
    auto func = [this] (std::function<void ()> t, int d, int p) {
        if(d > 0)
            std::this_thread::sleep_for(std::chrono::milliseconds(d));
        this->mActive = true;
        while(mActive) {
            auto startTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            t();
            auto endTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            auto elapsedTime = (int)(endTime - startTime);
            int remainSleepTime = p - elapsedTime;
            if(remainSleepTime > 0)
                std::this_thread::sleep_for(std::chrono::milliseconds(remainSleepTime));
        }
    };
    this->mThread = new std::thread(func, task, delay, period);
    this->mThread->detach();
}

EzyScheduleAtFixedRate::~EzyScheduleAtFixedRate() {
    mActive = false;
    EZY_SAFE_DELETE(mThread);
}

EZY_NAMESPACE_END_WITH
