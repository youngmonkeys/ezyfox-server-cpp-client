/*
 * AutoReleasePool.cpp
 *
 *  Created on: Jun 4, 2016
 *      Author: Quyet Nguyen
 */

#include "EzyRef.h"
#include "../pool/EzyAutoReleasePool.h"

EZY_NAMESPACE_START
namespace entity {

EzyRef::EzyRef(){
	retainCount = 1;
}

EzyRef::~EzyRef(){
}

void EzyRef::retain(){
	mMutex.lock();
	retainCount++;
	mMutex.unlock();
}

void EzyRef::release(){
	mMutex.lock();
	retainCount--;
	if (retainCount <= 0){
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
