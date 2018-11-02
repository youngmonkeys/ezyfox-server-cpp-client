#pragma once

#include <queue>
#include <ctime>
#include <vector>
#include "EzySocketCore.h"
#include "EzySocketPool.h"
#include "../base/EzyRef.h"
#include "../gc/EzyReleasePool.h"

EZY_NAMESPACE_START_WITH(socket)

class EzySocketAdapter {
protected:
    std::atomic_bool mStopped;
    std::atomic_bool mActive;
	EzySocketPool* mSocketPool;
protected:
	virtual void update();
public:
	EzySocketAdapter();
	virtual ~EzySocketAdapter();
	virtual void start();
    virtual void run();
	virtual void stop();
    virtual bool isActive();
    virtual bool isStopped();
    virtual void setActive(bool active);
    virtual void setStopped(bool stopped);
	virtual void pushMessage(EzySocketData* data);
    virtual EzySocketData* popMessage();
    virtual void popMessages(std::vector<EzySocketData*>& buffer);
};

EZY_NAMESPACE_END_WITH
