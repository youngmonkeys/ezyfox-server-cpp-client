#pragma once

#include <map>
#include "../EzyMacro.h"

EZY_NAMESPACE_START_WITH_ONLY(base)
class EzyRef;
EZY_NAMESPACE_END_WITH

EZY_NAMESPACE_START_WITH(gc)

class EzyReleasePool;

class EzyAutoReleasePool {
private:
    std::mutex mPoolMutex;
    std::map<size_t, EzyReleasePool*> mPools;
public:
    EZY_SINGLETON_GET_INSTANCE(EzyAutoReleasePool)
public:
    virtual void removePool();
    virtual EzyReleasePool* getPool();
};

EZY_NAMESPACE_END_WITH
