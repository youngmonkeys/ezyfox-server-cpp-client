#pragma once

#include <map>
#include "../EzyMacro.h"

#ifdef EZY_DEBUG
#include <string>
#endif

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
    void removePool();
    EzyReleasePool* getPool();
public:
#ifdef EZY_DEBUG
    EzyReleasePool* newPool(std::string name);
#endif
};

EZY_NAMESPACE_END_WITH
