#ifndef EzySocketPool_h
#define EzySocketPool_h

#include <queue>
#include <vector>
#include <condition_variable>
#include "EzySocketCore.h"

EZY_NAMESPACE_START_WITH(socket)

class EzySocketPacket;

class EzySocketPool {
protected:
    bool mDestroyed;
    std::mutex mPoolMutex;
    std::condition_variable mPoolCondition;
    std::queue<EzySocketPacket*> mDataQueue;
public:
    EzySocketPool();
    virtual ~EzySocketPool();
    
    virtual EzySocketPacket* take();
    virtual EzySocketData* pop();
    virtual void popAll(std::vector<EzySocketData*>& buffer);
    virtual void push(EzySocketData* data);
    virtual void offer(EzySocketData* data, bool encrypted);
    virtual void clear();
    virtual void destroy();
protected:
    virtual void doClear();
};

EZY_NAMESPACE_END_WITH

#endif /* EzySocketPool_h */
