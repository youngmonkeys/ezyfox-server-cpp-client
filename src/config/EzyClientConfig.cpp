#include "EzyClientConfig.h"

EZY_NAMESPACE_START_WITH_ONLY(config)

EzyClientConfig* EzyClientConfig::create() {
    auto pRet = new EzyClientConfig();
    return pRet;
}

EzyClientConfig::EzyClientConfig() {
    this->mClientName = "";
    this->mPing = new EzyPingConfig();
    this->mSocket = new EzySocketConfig();
    this->mReconnect = new EzyReconnectConfig();
}

EzyClientConfig::~EzyClientConfig() {
    EZY_SAFE_DELETE(mSocket);
    EZY_SAFE_DELETE(mPing);
    EZY_SAFE_DELETE(mReconnect);
}

std::string EzyClientConfig::getClientName() {
    if(mClientName.empty())
        return mZoneName;
    return mClientName;
}

void EzyClientConfig::setSocket(EzySocketConfig *socket) {
    EZY_SAFE_DELETE(mSocket);
    this->mSocket = socket;
}

void EzyClientConfig::setPing(EzyPingConfig* ping) {
    EZY_SAFE_DELETE(mPing);
    this->mPing = ping;
}

void EzyClientConfig::setReconnect(EzyReconnectConfig* reconnect) {
    EZY_SAFE_DELETE(mReconnect);
    this->mReconnect = reconnect;
}

//============================

EzySocketConfig::EzySocketConfig() {
    this->mReadBufferSize = 8 * 1024;
    this->mReadReserveSize = 16 * 1024;
    this->mEncodeReserveSize = 16 * 1024;
    this->mDecodeReserveSize = 16 * 1024;
    this->mUdpDecodeReserveSize = 8 * 1024;
}

//============================
EzyPingConfig::EzyPingConfig() {
    this->mPingPeriod = 3000;
    this->mMaxLostPingCount = 5;
}

//============================

EzyReconnectConfig::EzyReconnectConfig() {
    this->mEnable = true;
    this->mReconnectPeriod = 5000;
    this->mMaxReconnectCount = 5;
}


EZY_NAMESPACE_END_WITH
