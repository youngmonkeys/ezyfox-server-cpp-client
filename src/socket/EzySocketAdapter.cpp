#include <thread>
#include <chrono>
#include "EzySocketAdapter.h"
#include "../logger/EzyLogger.h"
#include "../entity/EzyValue.h"
#include "../entity/EzyJson.h"
#include "../entity/EzyObject.h"
#include "../gc/EzyAutoReleasePool.h"

EZY_NAMESPACE_START_WITH(socket)

EzySocketAdapter::EzySocketAdapter() {
	mRunning = false;
	mSocketPool = 0;
}

EzySocketAdapter::~EzySocketAdapter() {
    EZY_SAFE_DELETE(mSocketPool)
}

void EzySocketAdapter::updateThread(){
	this->update();
    gc::EzyAutoReleasePool::getInstance()->removePool();
	this->release();
}

bool EzySocketAdapter::isRunning() {
	std::unique_lock<std::mutex> lk(mMutex);
	return mRunning;
}

void EzySocketAdapter::setRunning(bool running) {
	std::unique_lock<std::mutex> lk(mMutex);
	this->mRunning = running;
}

void EzySocketAdapter::start() {
	if (!isRunning()){
		this->setRunning(true);
		this->retain();
		std::thread newThread(&EzySocketAdapter::updateThread, this);
		newThread.detach();
	}
}

void EzySocketAdapter::stop(){
	this->setRunning(false);
	mSocketPool->clear();
}

void EzySocketAdapter::update(){
}

void EzySocketAdapter::pushMessage(EzySocketData* data){
	mSocketPool->push(data);
}

EzySocketData* EzySocketAdapter::popMessage(){
	return mSocketPool->pop();
}

EZY_NAMESPACE_END_WITH
