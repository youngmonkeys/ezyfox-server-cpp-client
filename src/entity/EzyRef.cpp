#include "EzyRef.h"
#include "../gc/EzyAutoReleasePool.h"

EZY_NAMESPACE_START
namespace base {

EzyRef::EzyRef(){
	mRetainCount = 1;
}

EzyRef::~EzyRef(){
}

void EzyRef::retain(){
	mMutex.lock();
	mRetainCount++;
	mMutex.unlock();
}

void EzyRef::release(){
	mMutex.lock();
	mRetainCount--;
	if (mRetainCount <= 0){
		mMutex.unlock();
		delete this;
		return;
	}
	mMutex.unlock();
}

void EzyRef::autorelease(){
    auto pool = gc::EzyAutoReleasePool::getInstance()->getPool();
	pool->addObject(this);
	this->release();
}
	
}
EZY_NAMESPACE_END
