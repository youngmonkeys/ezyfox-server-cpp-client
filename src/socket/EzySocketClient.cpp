#include <thread>
#include "EzySocketClient.h"
#include "EzyPingSchedule.h"
#include "../event/EzyEvent.h"
#include "../gc/EzyAutoReleasePool.h"
#include "../manager/EzyHandlerManager.h"
#include "../manager/EzyPingManager.h"
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
    mPingManager = 0;
    mHandlerManager = 0;
    mStatus = SocketNotConnect;
    mDisconnectReason = constant::UnknownDisconnection;
    mConnectionFailedReason = constant::UnknownFailure;
    mSocketEventQueue = new EzySocketEventQueue();
}

EzySocketClient::~EzySocketClient() {
    closeClient();
    clearAdapters();
    mPingSchedule = 0;
    mReconnectConfig = 0;
    mLocalEventQueue.clear();
    mLocalMessageQueue.clear();
    EZY_SAFE_DELETE(mSocketEventQueue);
}

void EzySocketClient::setPingSchedule(EzyPingSchedule* pingSchedule) {
    pingSchedule->setSocketEventQueue(mSocketEventQueue);
    mPingSchedule = pingSchedule;
}

void EzySocketClient::setHandlerManager(manager::EzyHandlerManager* handlerManager) {
    mHandlerManager = handlerManager;
    mEventHandlers = handlerManager->getEventHandlers();
    mDataHandlers = handlerManager->getDataHandlers();
}

void EzySocketClient::connectTo(const std::string& host, int port) {
    setStatus(SocketConnecting);
    mHost = host;
    mPort = port;
    mReconnectCount = 0;
    connect0(0);
}

bool EzySocketClient::reconnect() {
    auto maxReconnectCount = mReconnectConfig->getMaxReconnectCount();
    if (mReconnectCount >= maxReconnectCount)
        return false;
    setStatus(SocketReconnecting);
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
    auto realSleepTime = sleepTime;
    if(sleepTime <= 0) {
        if(dt < 2000) //delay 2000ms
            realSleepTime = 2000 - dt;
    }
    if (realSleepTime >= 0)
        std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime - dt));
    setStatus(SocketConnecting);
    bool success = this->connectNow();
    mConnectTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    
    if (success) {
        this->setStatus(SocketConnected);
        this->mReconnectCount = 0;
        this->startAdapters();
        auto evt = event::EzyConnectionSuccessEvent::create();
        this->mSocketEventQueue->addEvent(evt);
    }
    else {
        this->setStatus(SocketNotConnect);
        this->resetSocket();
        auto evt = event::EzyConnectionFailureEvent::create(mConnectionFailedReason);
        this->mSocketEventQueue->addEvent(evt);
    }
}

bool EzySocketClient::connectNow() {
    return false;
}

void EzySocketClient::createAdapters() {
}

void EzySocketClient::startAdapters() {
}

void EzySocketClient::stopAdapter(EzySocketAdapter* adapter) {
    if(adapter)
        adapter->stop();
}

void EzySocketClient::clearAdapters() {
    std::unique_lock<std::mutex> lk(mClientMutex);
    clearAdapter(mSocketReader);
    clearAdapter(mSocketWriter);
}

void EzySocketClient::clearAdapter(EzySocketAdapter* adapter) {
    EZY_SAFE_DELETE(adapter);
}

void EzySocketClient::closeClient() {
    stopAdapter(mSocketWriter);
    stopAdapter(mSocketReader);
    mPingSchedule->stop();
    mSocketEventQueue->clear();
    closeSocket();
}

void EzySocketClient::resetSocket() {
}

void EzySocketClient::closeSocket() {
}

bool EzySocketClient::isConnectable() {
    std::unique_lock<std::mutex> lk(mStatusMutex);
    return mStatus == SocketNotConnect || mStatus == SocketDisconnected;
}

void EzySocketClient::setStatus(EzySocketStatus value) {
    std::unique_lock<std::mutex> lk(mStatusMutex);
    mStatus = value;
}

EzySocketStatus EzySocketClient::getStatus() {
    std::unique_lock<std::mutex> lk(mStatusMutex);
    return mStatus;
}

void EzySocketClient::onDisconnected(int reason) {
    setStatus(SocketDisconnected);
    auto event = event::EzyDisconnectionEvent::create(reason);
    mSocketEventQueue->addEvent(event);
}

void EzySocketClient::onDisconnect(int reason) {
    if(getStatus() != SocketConnected)
        return;
    setStatus(SocketDisconnecting);
    mDisconnectReason = reason;
    closeClient();
}

void EzySocketClient::sendMessage(EzySocketData* message) {
    mSocketWriter->pushMessage(message);
}

void EzySocketClient::processEventMessages() {
    processEvents();
    processReceivedMessages();
    mReleasePool = gc::EzyAutoReleasePool::getInstance()->getPool();
    mReleasePool->releaseAll();
}

void EzySocketClient::processEvents() {
    mSocketEventQueue->popAll(mLocalEventQueue);
    for (int i = 0; i < mLocalEventQueue.size(); ++i) {
        auto event = mLocalEventQueue[i];
        mEventHandlers->handle(event);
    }
    mLocalEventQueue.clear();
}

void EzySocketClient::processReceivedMessages() {
    if(getStatus() == SocketConnected) {
        if(mSocketReader->isStopped()) {
            onDisconnect(constant::UnknownDisconnection);
        }
        else if(mSocketReader->isActive()) {
            processReceivedMessages0();
        }
    }
    if(getStatus() == SocketDisconnecting) {
        if(mSocketWriter->isStopped()) {
            onDisconnected(mDisconnectReason);
        }
    }
}

void EzySocketClient::processReceivedMessages0() {
    mPingManager->setLostPingCount(0);
    mSocketReader->popMessages(mLocalMessageQueue);
    for(int i = 0 ; i < mLocalMessageQueue.size() ; ++i) {
        processReceivedMessage(mLocalMessageQueue[i]);
    }
    mLocalMessageQueue.clear();
}

void EzySocketClient::processReceivedMessage(EzySocketData* message) {
    auto array = (entity::EzyArray*)message;
    auto cmdId = array->getInt(0);
    auto data = array->getArray(1, 0);
    auto cmd = (constant::EzyCommand)cmdId;
    printReceivedData(cmd, data);
    if(cmd == constant::Disconnect) {
        auto reasonId = data->getInt(0);
        onDisconnected((int)reasonId);
    }
    else {
        mDataHandlers->handle(cmd, data);
    }
}

void EzySocketClient::printReceivedData(int cmd, entity::EzyArray* data) {
#ifdef EZY_DEBUG
    if(mUnloggableCommands.count(cmd) > 0)
        return;
    auto cmdName = constant::getCommandName(cmd);
    std::ostringstream stream;
    stream << "\n-------------------\n";
    stream << "[RECV] <== \n";
    stream << "command: ";
    stream << cmdName;
    stream << ", data:\n";
    logger::console(stream.str().c_str());
    stream.str("");
    stream.clear();
    if(data)
        data->printDebug();
    logger::console("\n-------------------\n");
#endif
}

EZY_NAMESPACE_END_WITH
