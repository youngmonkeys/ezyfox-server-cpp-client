#pragma once

#include <queue>
#include <vector>
#include "EzySocketCore.h"

EZY_NAMESPACE_START_WITH(socket)

class EzySocketPool {
protected:
    std::mutex mPoolMutex;
    std::condition_variable mPoolCondition;
    std::queue<EzySocketData*> mDataQueue;
public:
    EzySocketPool();
    virtual ~EzySocketPool();
    
    virtual EzySocketData* take();
    virtual EzySocketData* pop();
    virtual void popAll(std::vector<EzySocketData*>& buffer);
    virtual void push(EzySocketData* data);
    virtual void clear();
protected:
    virtual void clear0();
};

EZY_NAMESPACE_END_WITH
