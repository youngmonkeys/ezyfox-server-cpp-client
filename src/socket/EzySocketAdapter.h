#pragma once

#include <queue>
#include <ctime>
#include <condition_variable>
#include "EzySocketCore.h"
#include "EzySocketPool.h"
#include "../base/EzyRef.h"
#include "../gc/EzyReleasePool.h"

EZY_NAMESPACE_START_WITH(socket)

class EzySocketAdapter : public base::EzyRef {
protected:
    bool mActive;
	std::mutex mMutex;
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
    virtual void setActive(bool active);
    virtual EzySocketData* popMessage();
	virtual void pushMessage(EzySocketData* data);
};

EZY_NAMESPACE_END_WITH
