#pragma once

#include <queue>
#include "EzySocketCore.h"

EZY_NAMESPACE_START
namespace socket {

class EzySocketPool {
protected:
    std::mutex mPoolMutex;
    std::condition_variable mPoolCondition;
    std::queue<EzySocketData*>* mDataQueue;
public:
    EzySocketPool();
    virtual ~EzySocketPool();
    
    virtual EzySocketData* take();
    virtual EzySocketData* pop();
    virtual void push(EzySocketData* data);
    virtual void clear();
};

}
EZY_NAMESPACE_END
