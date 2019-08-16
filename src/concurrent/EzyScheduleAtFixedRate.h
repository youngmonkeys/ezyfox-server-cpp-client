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
#include <functional>
#include "../EzyMacro.h"
#include "../base/EzyRef.h"

EZY_NAMESPACE_START_WITH_ONLY(concurrent)

class EzyScheduleAtFixedRate : public base::EzyRef {
protected:
    bool mActive;;
    std::string mThreadName;
    std::mutex mScheduleMutex;
public:
    EzyScheduleAtFixedRate(std::string threadName = "");
    ~EzyScheduleAtFixedRate();
    void schedule(std::function<void(void)> task, int delay, int period);
    void stop();
    
protected:
    void startLoop(std::function<void(void)> task, int delay, int period);
    void startLoop0(std::function<void(void)> task, int delay, int period);
    bool stoppable();
};

EZY_NAMESPACE_END_WITH
