#ifndef EzyPingManager_h
#define EzyPingManager_h

#include <mutex>
#include "../EzyMacro.h"

EZY_NAMESPACE_START_WITH_ONLY(manager)

class EzyPingManager {
protected:
    int mLostPingCount;
    std::mutex lostPingCountMutex;
protected:
    EZY_SYNTHESIZE_READONLY(int, PingPeriod)
    EZY_SYNTHESIZE_READONLY(int, MaxLostPingCount)
public:
    EzyPingManager();
    int increaseLostPingCount();
    int getLostPingCount();
    void setLostPingCount(int count);
};

EZY_NAMESPACE_END_WITH

#endif /* EzyPingManager_h */
