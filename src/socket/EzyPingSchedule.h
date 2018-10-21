#pragma once

#include <thread>
#include "../EzyMacro.h"

EZY_NAMESPACE_START
class EzyClient;
EZY_NAMESPACE_END

EZY_NAMESPACE_START_WITH_ONLY(manager)
class EzyPingManager;
EZY_NAMESPACE_END_WITH

EZY_NAMESPACE_START_WITH_ONLY(socket)

class EzySender;

class EzyPingSchedule {
protected:
    
    bool mActive;
    EzyClient* mClient;
    std::thread* mThread;
    manager::EzyPingManager* mPingManager;
    
    void loop();
    void sendPingRequest();
public:
    EzyPingSchedule(EzyClient* mClient);
    ~EzyPingSchedule();
    void start();
    void stop();
};

EZY_NAMESPACE_END_WITH
