#pragma once

#include <thread>
#include "../EzyMacro.h"
#include "../request/EzyRequestDeliver.h"

EZY_NAMESPACE_START
namespace handler {

class EzyPingSchedule {
protected:
    
    std::thread* mThread;
    request::EzyRequestDeliver* mRequestDeliver;
    EZY_SYNTHESIZE(int, Period);
    EZY_SYNTHESIZE_BOOL_READONLY(Active);
    
    void sendPingRequest();
    void loopSendPingRequest();
public:
    EzyPingSchedule(request::EzyRequestDeliver* mRequestDeliver);
    ~EzyPingSchedule();
    void start();
    void stop();
};
    
}
EZY_NAMESPACE_END
