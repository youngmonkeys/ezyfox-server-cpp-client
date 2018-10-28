#include "EzyClientConfig.h"

EZY_NAMESPACE_START_WITH_ONLY(config)

EzyClientConfig* EzyClientConfig::create() {
    auto pRet = new EzyClientConfig();
    return pRet;
}

EzyClientConfig::EzyClientConfig() {
    this->mClientName = "";
    this->mReconnect = new EzyReconnectConfig();
}

std::string EzyClientConfig::getClientName() {
    if(mClientName.empty())
        return mZoneName;
    return mClientName;
}

void EzyClientConfig::setReconnect(EzyReconnectConfig* reconnect) {
    EZY_SAFE_DELETE(mReconnect);
    this->mReconnect = reconnect;
}

EzyClientConfig::~EzyClientConfig() {
    EZY_SAFE_DELETE(mReconnect);
}

EzyReconnectConfig::EzyReconnectConfig() {
    this->mEnable = true;
    this->mReconnectPeriod = 5000;
    this->mMaxReconnectCount = 5;
}


EZY_NAMESPACE_END_WITH
