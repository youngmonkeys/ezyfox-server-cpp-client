#pragma once

#include <queue>
#include <ctime>
#include <condition_variable>
#include "EzySocketCore.h"
#include "EzySocketPool.h"
#include "../base/EzyRef.h"
#include "../gc/EzyReleasePool.h"

EZY_NAMESPACE_START_WITH(socket)
    
class EzySocketDataHandler {
public:
    virtual ~EzySocketDataHandler() {}
    virtual void handleSocketData(EzySocketData* data) = 0;
};
    
class EzySocketStatusHandler {
public:
    virtual ~EzySocketStatusHandler() {}
    virtual void handleSocketStatus(const EzySocketStatusData& data) = 0;
};

class EzySocketAdapter : public base::EzyRef {
    
protected:
    
	bool mRunning;
	std::mutex mMutex;
	EzySocketPool* mSocketPool;
    
	virtual void update();
    
public:
    
	EzySocketAdapter();
	virtual ~EzySocketAdapter();

	virtual void updateThread();

	virtual bool isRunning();
	virtual void setRunning(bool running);

	virtual void start();
	virtual void stop();

	virtual void pushMessage(EzySocketData* data);
	virtual EzySocketData* popMessage();
};

EZY_NAMESPACE_END_WITH
