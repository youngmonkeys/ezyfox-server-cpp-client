//
//  EzyScheduleAtFixedRate.h
//  ezyfox-server-cpp-client
//
//  Created by Dung Ta Van on 8/4/19.
//  Copyright © 2019 Young Monkeys. All rights reserved.
//

#ifndef EzyScheduleAtFixedRate_h
#define EzyScheduleAtFixedRate_h

#include <string>
#include <mutex>
#include <functional>
#include <condition_variable>
#include "../EzyMacro.h"
#include "../base/EzyRef.h"

EZY_NAMESPACE_START_WITH_ONLY(concurrent)

class EzyScheduleAtFixedRate : public base::EzyRef {
protected:
    bool mActive;
    std::string mThreadName;
    std::mutex mScheduleMutex;
    std::mutex mSleepMutex;
    std::condition_variable mSleepCondition;
public:
    EzyScheduleAtFixedRate(std::string threadName = "");
    ~EzyScheduleAtFixedRate();
    void schedule(std::function<void(void)> task, int delay, int period);
    void stop();
    
protected:
    void startLoop(std::function<void(void)> task, int delay, int period);
    void startLoop0(std::function<void(void)> task, int delay, int period);
    bool stoppable();
    void sleep(int time);
    void wakeup();
};

EZY_NAMESPACE_END_WITH

#endif /* EzyScheduleAtFixedRate_h */
