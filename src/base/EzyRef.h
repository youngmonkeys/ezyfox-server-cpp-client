#ifndef EzyRef_h
#define EzyRef_h

#include <mutex>
#include "../EzyMacro.h"

EZY_NAMESPACE_START_WITH_ONLY(base)

class EzyRef {
protected:
	int mRetainCount;
	std::mutex mMutex;
public:
	EzyRef();
	virtual ~EzyRef();
	virtual void retain();
	virtual void release();
	virtual void autorelease();
};

EZY_NAMESPACE_END_WITH

#endif /* EzyRef_h */
