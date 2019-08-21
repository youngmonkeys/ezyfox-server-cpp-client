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
	virtual void update();
public:
	EzySocketAdapter();
	virtual ~EzySocketAdapter();
	virtual void start();
    virtual void run();
	virtual void stop();
    virtual void setActive(bool active);
    virtual void setStopped(bool stopped);
    virtual bool isActive();
    virtual bool isStopped();
	virtual void pushMessage(EzySocketData* data);
    virtual EzySocketData* popMessage();
    virtual void popMessages(std::vector<EzySocketData*>& buffer);
};

EZY_NAMESPACE_END_WITH
