#include "EzyPingManager.h"

EZY_NAMESPACE_START_WITH(manager)

EzyPingManager::EzyPingManager() {
    this->mPingPeriod = 5000;
    this->mLostPingCount = 0;
    this->mMaxLostPingCount = 5;
}

int EzyPingManager::increaseLostPingCount() {
    std::unique_lock<std::mutex> lock(lostPingCountMutex);
    this->mLostPingCount ++;
    return this->mLostPingCount;
}

int EzyPingManager::getLostPingCount() {
    std::unique_lock<std::mutex> lock(lostPingCountMutex);
    return this->mLostPingCount;
}

void EzyPingManager::setLostPingCount(int count) {
    std::unique_lock<std::mutex> lock(lostPingCountMutex);
    this->mLostPingCount = count;
}

EZY_NAMESPACE_END_WITH
