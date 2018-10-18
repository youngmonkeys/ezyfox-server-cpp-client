#pragma once

#include <thread>
#include "../EzyMacro.h"

EZY_NAMESPACE_START_WITH_ONLY(manager)
class EzyPingManager;
EZY_NAMESPACE_END_WITH

EZY_NAMESPACE_START_WITH_ONLY(socket)

class EzySender;

class EzyPingSchedule {
protected:
    
    bool mActive;
    EzySender* mSender;
    std::thread* mThread;
    manager::EzyPingManager* mPingManager;
    
    void loop();
    void sendPingRequest();
public:
    EzyPingSchedule(EzySender* sender, manager::EzyPingManager* pingManager);
    ~EzyPingSchedule();
    void start();
    void stop();
};

EZY_NAMESPACE_END_WITH
