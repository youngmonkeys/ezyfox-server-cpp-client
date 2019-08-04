#pragma once

#include <mutex>
#include "../EzyMacro.h"

EZY_NAMESPACE_START
class EzyClient;
EZY_NAMESPACE_END

EZY_NAMESPACE_START_WITH_ONLY(concurrent)
class EzyScheduleAtFixedRate;
EZY_NAMESPACE_END_WITH

EZY_NAMESPACE_START_WITH_ONLY(manager)
class EzyPingManager;
EZY_NAMESPACE_END_WITH

EZY_NAMESPACE_START_WITH_ONLY(socket)

class EzySender;
class EzySocketEventQueue;

class EzyPingSchedule {
protected:
    std::mutex mMutex;
    EzyClient* mClient;
    manager::EzyPingManager* mPingManager;
    concurrent::EzyScheduleAtFixedRate* mSchedule;
protected:
    EZY_SYNTHESIZE_WRITEONLY(socket::EzySocketEventQueue*, SocketEventQueue);
protected:
    void sendPingRequest();
public:
    EzyPingSchedule(EzyClient* mClient);
    ~EzyPingSchedule();
    void start();
    void stop();
};

EZY_NAMESPACE_END_WITH
