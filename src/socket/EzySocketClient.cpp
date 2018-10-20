#include <thread>
#include "EzySocketClient.h"
#include "../event/EzyEvent.h"
#include "../gc/EzyAutoReleasePool.h"
#include "../manager/EzyHandlerManager.h"
#include "../handler/EzyEventHandlers.h"
#include "../handler/EzyDataHandlers.h"
#include "../constant/EzyDisconnectReason.h"
#include "../constant/EzyConnectionFailedReason.h"
#include "../config/EzyClientConfig.h"
#include "../logger/EzyLogger.h"

EZY_NAMESPACE_START_WITH(socket)

EzySocketClient::EzySocketClient() {
    mPort = 0;
    mHost = "";
    mConnectTime = 0;
    mSocketReader = 0;
    mSocketWriter = 0;
    mReleasePool = 0;
    mReconnectCount = 0;
    mReconnectConfig = 0;
    mHandlerManager = 0;
}

EzySocketClient::~EzySocketClient() {
    this->closeSocket();
    this->clearAdapter();
}

void EzySocketClient::setReconnectConfig(config::EzyReconnectConfig *reconnectConfig) {
    this->mReconnectConfig = reconnectConfig;
}

void EzySocketClient::setHandlerManager(manager::EzyHandlerManager* handlerManager) {
    this->mHandlerManager = handlerManager;
    this->mEventHandlers = handlerManager->getEventHandlers();
    this->mDataHandlers = handlerManager->getDataHandlers();
}

void EzySocketClient::closeSocket() {
}

void EzySocketClient::createAdapter() {
}

bool EzySocketClient::connectNow() {
    return false;
}

void EzySocketClient::connectTo(const std::string& host, int port) {
    clearAdapter();
    createAdapter();
    closeSocket();
    mSocketEventQueue.clear();
    mHost = host;
    mPort = port;
    mReconnectCount = 0;
    retain();
    std::thread newThread(&EzySocketClient::connect0, this, 0);
    newThread.detach();
}

bool EzySocketClient::reconnect() {
    auto maxReconnectCount = mReconnectConfig->getMaxReconnectCount();
    if (mReconnectCount >= maxReconnectCount)
        return false;
    auto reconnectSleepTime = mReconnectConfig->getReconnectPeriod();
    connect0(reconnectSleepTime);
    mReconnectCount++;
    logger::log("try reconnect to server: %d, wating time: %d", mReconnectCount, reconnectSleepTime);
    auto tryConnectEvent = event::EzyTryConnectEvent::create(mReconnectCount);
    mSocketEventQueue.addEvent(tryConnectEvent);
    return true;
}

void EzySocketClient::connect0(long sleepTime) {
    auto currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    auto dt = currentTime - mConnectTime;
    if (dt < sleepTime) { //delay 2000ms
        std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime - dt));
    }
    
    bool success = this->connectNow();
    mConnectTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    
    if (success) {
        this->startAdapter();
        this->mReconnectCount = 0;
        auto evt = event::EzyConnectionSuccessEvent::create();
        this->mSocketEventQueue.addEvent(evt);
    }
    else {
        this->resetSocket();
        auto evt = event::EzyConnectionFailureEvent::create(constant::EzyConnectionFailedReason::UnknownFailure);
        this->mSocketEventQueue.addEvent(evt);
    }
    
    gc::EzyAutoReleasePool::getInstance()->removePool();
    this->release();
}

void EzySocketClient::startAdapter() {
    
}

void EzySocketClient::closeClient() {
    if (mSocketWriter) {
        mSocketWriter->stop();
    }
    if(mSocketReader) {
        mSocketReader->stop();
    }
    mSocketEventQueue.clear();
    mSocketEventQueue.addEvent(event::EzyConnectionClosedEvent::create());
    closeSocket();
}

void EzySocketClient::sendMessage(EzySocketData* data) {
    if (mSocketWriter){
        mSocketWriter->pushMessage(data);
    }
}

void EzySocketClient::processEvent() {
    mSocketEventQueue.popAll(mLocalEventQueue);
    for (int i = 0; i < mLocalEventQueue.size(); i++) {
        auto event = mLocalEventQueue[i];
        mEventHandlers->handle(event);
    }
    mLocalEventQueue.clear();
}

void EzySocketClient::processReceivedMessage() {
    auto data = mSocketReader->popMessage();
    while (data){
        mDataHandlers->handle((entity::EzyArray*)data);
        data = mSocketReader->popMessage();
    }
}

void EzySocketClient::clearAdapter() {
    std::unique_lock<std::mutex> lk(mClientMutex);
    if (mSocketReader) {
        mSocketReader->stop();
        mSocketReader->release();
        mSocketReader = 0;
    }
    if (mSocketWriter) {
        mSocketWriter->stop();
        mSocketWriter->release();
        mSocketWriter = 0;
    }
}

void EzySocketClient::resetSocket() {
}

void EzySocketClient::processSocketError() {
    auto evt = event::EzyDisconnectionEvent::create(constant::EzyDisconnectReason::UnknownDisconnection);
    this->mSocketEventQueue.addEvent(evt);
}

void EzySocketClient::processMessage() {
    processEvent();
    processReceivedMessage();
    mReleasePool = gc::EzyAutoReleasePool::getInstance()->getPool();
    mReleasePool->releaseAll();
}

EZY_NAMESPACE_END_WITH
