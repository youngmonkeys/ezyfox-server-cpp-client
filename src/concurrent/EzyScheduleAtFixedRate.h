//
//  EzyScheduleAtFixedRate.hpp
//  ezyfox-server-cpp-client
//
//  Created by Dung Ta Van on 8/4/19.
//  Copyright © 2019 Young Monkeys. All rights reserved.
//

#pragma once

#include <string>
#include <atomic>
#include <thread>
#include <functional>
#include "../EzyMacro.h"

EZY_NAMESPACE_START_WITH_ONLY(concurrent)

class EzyScheduleAtFixedRate {
protected:
    bool mActive;
    std::mutex mMutex;
    std::thread mThread;
    std::string mThreadName;
public:
    EzyScheduleAtFixedRate(std::string threadName = "");
    
    void schedule(std::function<void(void)> task, int delay, int period);
    void stop();
    
protected:
    bool stoppable();
};

EZY_NAMESPACE_END_WITH
