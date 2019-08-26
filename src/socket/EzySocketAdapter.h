#pragma once

#include <vector>
#include <mutex>
#include "EzySocketCore.h"
#include "EzySocketPool.h"
#include "../base/EzyRef.h"
#include "../gc/EzyReleasePool.h"

EZY_NAMESPACE_START_WITH(socket)

class EzySocketAdapter : public base::EzyRef {
protected:
    bool mActive;
    bool mStopped;
    std::mutex mAdapterMutex;
	EzySocketPool* mSocketPool;
protected:
    virtual void run();
	virtual void update();
    virtual void setActive(bool active);
    virtual void setStopped(bool stopped);
public:
	EzySocketAdapter();
	virtual ~EzySocketAdapter();
	virtual void start();
	virtual void stop();
    virtual bool isActive();
    virtual bool isStopped();
};

EZY_NAMESPACE_END_WITH
