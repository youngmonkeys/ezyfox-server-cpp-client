#include "EzyPingManager.h"
#include "../config/EzyClientConfig.h"

EZY_NAMESPACE_START_WITH(manager)

EzyPingManager::EzyPingManager(config::EzyPingConfig *config) {
    this->mLostPingCount = 0;
    this->mPingPeriod = config->getPingPeriod();
    this->mMaxLostPingCount = config->getMaxLostPingCount();
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
