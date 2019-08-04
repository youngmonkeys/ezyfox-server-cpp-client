//
//  EzyScheduleAtFixedRate.hpp
//  ezyfox-server-cpp-client
//
//  Created by Dung Ta Van on 8/4/19.
//  Copyright © 2019 Young Monkeys. All rights reserved.
//

#pragma once

#include <atomic>
#include <thread>
#include <functional>
#include "../EzyMacro.h"

EZY_NAMESPACE_START_WITH_ONLY(concurrent)

class EzyScheduleAtFixedRate {
protected:
    std::atomic<bool> mActive;
    std::atomic<bool> mStarted;
    std::thread* mThread;
public:
    EzyScheduleAtFixedRate();
    ~EzyScheduleAtFixedRate();
    
    void schedule(std::function<void(void)> task, int delay, int period);
};

EZY_NAMESPACE_END_WITH
