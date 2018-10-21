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
    mSocketEventQueue = new EzySocketEventQueue();
}

EzySocketClient::~EzySocketClient() {
    closeSocket();
    clearAdapters();
    mLocalEventQueue.clear();
    EZY_SAFE_DELETE(mSocketEventQueue);
}

void EzySocketClient::setReconnectConfig(config::EzyReconnectConfig *reconnectConfig) {
    mReconnectConfig = reconnectConfig;
}

void EzySocketClient::setHandlerManager(manager::EzyHandlerManager* handlerManager) {
    mHandlerManager = handlerManager;
    mEventHandlers = handlerManager->getEventHandlers();
    mDataHandlers = handlerManager->getDataHandlers();
}

void EzySocketClient::closeSocket() {
}

void EzySocketClient::createAdapters() {
}

bool EzySocketClient::connectNow() {
    return false;
}

void EzySocketClient::connectTo(const std::string& host, int port) {
    mHost = host;
    mPort = port;
    mReconnectCount = 0;
    connect0(0);
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
    mSocketEventQueue->addEvent(tryConnectEvent);
    return true;
}

void EzySocketClient::connect0(long sleepTime) {
    clearAdapters();
    createAdapters();
    closeSocket();
    mSocketEventQueue->clear();
    std::thread newThread(&EzySocketClient::connect1, this, sleepTime);
    newThread.detach();
}

void EzySocketClient::connect1(long sleepTime) {
    auto currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    auto dt = currentTime - mConnectTime;
    if (dt < sleepTime) { //delay 2000ms
        std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime - dt));
    }
    
    bool success = this->connectNow();
    mConnectTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    
    if (success) {
        this->startAdapters();
        this->mReconnectCount = 0;
        auto evt = event::EzyConnectionSuccessEvent::create();
        this->mSocketEventQueue->addEvent(evt);
    }
    else {
        this->resetSocket();
        auto evt = event::EzyConnectionFailureEvent::create(mConnectionFailedReason);
        this->mSocketEventQueue->addEvent(evt);
    }
}

void EzySocketClient::startAdapters() {
}

void EzySocketClient::closeClient() {
    stopAdapter(mSocketWriter);
    stopAdapter(mSocketReader);
    mSocketEventQueue->clear();
    closeSocket();
}

void EzySocketClient::stopAdapter(EzySocketAdapter* adapter) {
    if(adapter)
        adapter->stop();
}

void EzySocketClient::sendMessage(EzySocketData* message) {
    if (mSocketWriter) {
        mSocketWriter->pushMessage(message);
    }
}

void EzySocketClient::processEvents() {
    mSocketEventQueue->popAll(mLocalEventQueue);
    for (int i = 0; i < mLocalEventQueue.size(); i++) {
        auto event = mLocalEventQueue[i];
        mEventHandlers->handle(event);
    }
    mLocalEventQueue.clear();
}

void EzySocketClient::processReceivedMessages() {
    auto data = mSocketReader->popMessage();
    while (data) {
        processReceivedMessage(data);
        data = mSocketReader->popMessage();
    }
}

void EzySocketClient::processReceivedMessage(EzySocketData* message) {
    auto array = (entity::EzyArray*)message;
    auto cmdId = array->getInt(0);
    auto data = array->getArray(1);
    auto cmd = (constant::EzyCommand)cmdId;
    if(cmd == constant::Disconnect) {
        auto reasonId = data->getInt(0);
        onDisconnected((int)reasonId);
    }
    else {
        mDataHandlers->handle(cmd, data);
    }
}

void EzySocketClient::onDisconnected(int reason) {
    auto event = event::EzyDisconnectionEvent::create(reason);
    mSocketEventQueue->addEvent(event);
}

void EzySocketClient::clearAdapters() {
    std::unique_lock<std::mutex> lk(mClientMutex);
    clearAdapter(mSocketReader);
    clearAdapter(mSocketWriter);
}

void EzySocketClient::clearAdapter(EzySocketAdapter* adapter) {
    if(adapter) {
        adapter->stop();
        adapter->release();
        adapter = 0;
    }
}

void EzySocketClient::resetSocket() {
}

void EzySocketClient::processEventMessages() {
    processEvents();
    processReceivedMessages();
    mReleasePool = gc::EzyAutoReleasePool::getInstance()->getPool();
    mReleasePool->releaseAll();
}

EZY_NAMESPACE_END_WITH
