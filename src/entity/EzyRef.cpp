#include "EzyRef.h"
#include "../pool/EzyAutoReleasePool.h"

EZY_NAMESPACE_START
namespace entity {

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
    auto pool = pool::EzyAutoReleasePool::getInstance()->getPool();
	pool->addObject(this);
	this->release();
}
	
}
EZY_NAMESPACE_END
