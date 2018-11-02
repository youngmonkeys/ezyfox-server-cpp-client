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
	mActive = false;
    mStopped = false;
	mSocketPool = 0;
}

EzySocketAdapter::~EzySocketAdapter() {
    mActive = false;
    EZY_SAFE_DELETE(mSocketPool)
}

void EzySocketAdapter::run() {
    setStopped(false);
	update();
    gc::EzyAutoReleasePool::getInstance()->removePool();
    setStopped(true);
}

bool EzySocketAdapter::isActive() {
	return mActive;
}

bool EzySocketAdapter::isStopped() {
    return mStopped;
}

void EzySocketAdapter::setActive(bool active) {
	mActive = active;
}

void EzySocketAdapter::setStopped(bool stopped) {
    mStopped = stopped;
}

void EzySocketAdapter::start() {
	if (!isActive()) {
		setActive(true);
		std::thread newThread(&EzySocketAdapter::run, this);
		newThread.detach();
	}
}

void EzySocketAdapter::stop() {
	mSocketPool->clear();
    setActive(false);
}

void EzySocketAdapter::update() {
}

void EzySocketAdapter::pushMessage(EzySocketData* data) {
	mSocketPool->push(data);
}

EzySocketData* EzySocketAdapter::popMessage() {
	return mSocketPool->pop();
}

void EzySocketAdapter::popMessages(std::vector<EzySocketData*>& buffer) {
    mSocketPool->popAll(buffer);
}

EZY_NAMESPACE_END_WITH
