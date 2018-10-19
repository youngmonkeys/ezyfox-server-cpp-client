#pragma once

#include <map>
#include "../EzyMacro.h"

EZY_NAMESPACE_START_WITH_ONLY(base)
class EzyRef;
EZY_NAMESPACE_END_WITH

EZY_NAMESPACE_START_WITH(gc)

class EzyReleasePool;

class EzyAutoReleasePool {
    std::mutex mPoolMutex;
    std::map<size_t, EzyReleasePool*> mPools;
public:
    EzyAutoReleasePool();
    virtual ~EzyAutoReleasePool();
    
    virtual void removePool();
    virtual EzyReleasePool* getPool();
    
    static EzyAutoReleasePool* getInstance();
};

EZY_NAMESPACE_END_WITH
