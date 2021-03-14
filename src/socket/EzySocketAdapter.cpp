#include <thread>
#include <chrono>
#include "EzySocketAdapter.h"
#include "../logger/EzyLogger.h"
#include "../entity/EzyValue.h"
#include "../entity/EzyObject.h"
#include "../gc/EzyAutoReleasePool.h"

EZY_NAMESPACE_START_WITH(socket)

EzySocketAdapter::EzySocketAdapter() {
	mActive = false;
    mStopped = false;
	mSocketPool = new EzySocketPool();
}

EzySocketAdapter::~EzySocketAdapter() {
    mActive = false;
    mStopped = true;
    EZY_SAFE_DELETE(mSocketPool)
}

void EzySocketAdapter::start() {
    std::unique_lock<std::mutex> lk(mAdapterMutex);
	if (!mActive) {
        mActive = true;
        mStopped = false;
        retain();
		std::thread newThread(&EzySocketAdapter::run, this);
		newThread.detach();
	}
}

void EzySocketAdapter::run() {
    update();
    gc::EzyAutoReleasePool::getInstance()->removePool();
    setStopped(true);
    release();
}

void EzySocketAdapter::stop() {
    std::unique_lock<std::mutex> lk(mAdapterMutex);
    mSocketPool->destroy();
    mActive = false;
}

void EzySocketAdapter::update() {
}

void EzySocketAdapter::setActive(bool active) {
    std::unique_lock<std::mutex> lk(mAdapterMutex);
    mActive = active;
}

void EzySocketAdapter::setStopped(bool stopped) {
    std::unique_lock<std::mutex> lk(mAdapterMutex);
    mStopped = stopped;
}

bool EzySocketAdapter::isActive() {
    std::unique_lock<std::mutex> lk(mAdapterMutex);
    return mActive;
}

bool EzySocketAdapter::isStopped() {
    std::unique_lock<std::mutex> lk(mAdapterMutex);
    return mStopped;
}

EZY_NAMESPACE_END_WITH
