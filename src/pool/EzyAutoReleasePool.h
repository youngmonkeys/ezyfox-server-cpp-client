#pragma once

#include "../EzyMacro.h"
#include "../entity/EzyRef.h"
#include "EzyReleasePool.h"

EZY_NAMESPACE_START
namespace pool {

class EzyAutoReleasePool {
    std::mutex poolMutex;
    std::map<size_t, EzyReleasePool*> pools;
public:
    EzyAutoReleasePool();
    virtual ~EzyAutoReleasePool();
    
    virtual void removePool();
    virtual EzyReleasePool* getPool();
    
    static EzyAutoReleasePool* getInstance();
};

}
EZY_NAMESPACE_END
