#include <thread>
#include "EzySocketClient.h"
#include "EzyPingSchedule.h"
#include "../util/EzyStack.h"
#include "../logger/EzyLogger.h"
#include "../event/EzyEvent.h"
#include "../gc/EzyAutoReleasePool.h"
#include "../concurrent/EzyThread.h"
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
    mConfig = 0;
    mConnectTime = 0;
    mSocketReader = 0;
    mSocketWriter = 0;
    mReleasePool = 0;
    mReconnectCount = 0;
    mReconnectConfig = 0;
    mPingManager = 0;
    mPingSchedule = 0;
    mHandlerManager = 0;
    mEventHandlers = 0;
    mDataHandlers = 0;
    mLocalEventQueue.clear();
    mLocalMessageQueue.clear();
    mLocalSocketStatuses.clear();
    mUnloggableCommands.clear();
    mDisconnectReason = constant::UnknownDisconnection;
    mConnectionFailedReason = constant::UnknownFailure;
    mSocketEventQueue = new EzySocketEventQueue();
    mSocketStatuses = new util::EzyStack<EzySocketStatus>(SocketNotConnect);
}

EzySocketClient::~EzySocketClient() {
    mPingSchedule = 0;
    mReconnectConfig = 0;
    mLocalEventQueue.clear();
    mLocalMessageQueue.clear();
    mSocketStatuses->clear();
    mSocketEventQueue->clear();
    EZY_SAFE_DELETE(mSocketStatuses);
    EZY_SAFE_DELETE(mSocketEventQueue);
    EZY_SAFE_DELETE(mSocketReader);
    EZY_SAFE_DELETE(mSocketWriter);
}

void EzySocketClient::destroy() {
    auto status = mSocketStatuses->last();
    if(!isSocketDestroyable(status)) {
        logger::log("socket is not in a destroyable status");
        return;
    }
    this->release();
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
    auto status = mSocketStatuses->last();
    if(!isSocketConnectable(status)) {
        logger::log("socket is connecting...");
        return;
    }
    mSocketStatuses->push(SocketConnecting);
    mHost = host;
    mPort = port;
    mReconnectCount = 0;
    connect0(0);
}

bool EzySocketClient::reconnect() {
    auto status = mSocketStatuses->last();
    if(!isSocketReconnectable(status)) {
        logger::log("socket is not in a reconnectable status");
        return false;
    }
    auto maxReconnectCount = mReconnectConfig->getMaxReconnectCount();
    if (mReconnectCount >= maxReconnectCount)
        return false;
    mSocketStatuses->push(SocketReconnecting);
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
    mSocketStatuses->clear();
    mDisconnectReason = constant::UnknownDisconnection;
    mConnectionFailedReason = constant::UnknownFailure;
    retain();
    std::thread newThread(&EzySocketClient::connect1, this, sleepTime);
    newThread.detach();
}

void EzySocketClient::connect1(long sleepTime) {
    concurrent::EzyThread::setCurrentThreadName("ezyfox-connection");
    auto currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    auto dt = currentTime - mConnectTime;
    auto realSleepTime = sleepTime;
    if(sleepTime <= 0) {
        if(dt < 2000) //delay 2000ms
            realSleepTime = 2000 - dt;
    }
    if (realSleepTime >= 0)
        std::this_thread::sleep_for(std::chrono::milliseconds(realSleepTime));
    mSocketStatuses->push(SocketConnecting);
    bool success = this->connectNow();
    mConnectTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    
    if (success) {
        this->mReconnectCount = 0;
        this->startAdapters();
        this->mSocketStatuses->push(SocketConnected);
    }
    else {
        this->resetSocket();
        this->mSocketStatuses->push(SocketConnectFailed);
    }
    gc::EzyAutoReleasePool::getInstance()->removePool();
    release();
}

bool EzySocketClient::connectNow() {
    return false;
}

void EzySocketClient::createAdapters() {
}

void EzySocketClient::startAdapters() {
}

void EzySocketClient::clearAdapters() {
    clearAdapter(mSocketReader);
    clearAdapter(mSocketWriter);
    mSocketReader = 0;
    mSocketWriter = 0;
}

void EzySocketClient::clearAdapter(EzySocketAdapter* adapter) {
    if(adapter) {
        adapter->stop();
        adapter->release();
    }
}

void EzySocketClient::clearComponents(int disconnectReason){
}

void EzySocketClient::resetSocket() {
}

void EzySocketClient::closeSocket() {
}

void EzySocketClient::onDisconnected(int reason) {
    mPingSchedule->stop();
    mSocketEventQueue->clear();
    closeSocket();
    clearAdapters();
    clearComponents(reason);
    mSocketStatuses->push(SocketDisconnected);
}

void EzySocketClient::disconnect(int reason) {
    if(mSocketStatuses->last() != SocketConnected)
        return;
    onDisconnected(mDisconnectReason = reason);
}

void EzySocketClient::sendMessage(EzySocketData* message) {
    mSocketWriter->offerMessage(message);
}

void EzySocketClient::processEventMessages() {
    processReceivedMessages();
    processStatuses();
    processEvents();
    mReleasePool = gc::EzyAutoReleasePool::getInstance()->getPool();
    mReleasePool->releaseAll();
}

void EzySocketClient::processStatuses() {
    mSocketStatuses->popAll(mLocalSocketStatuses);
    for (int i = 0 ; i < mLocalSocketStatuses.size() ; ++i) {
        auto status = mLocalSocketStatuses[i];
        if(status == SocketConnected) {
            auto evt = event::EzyConnectionSuccessEvent::create();
            this->mSocketEventQueue->addEvent(evt);
        }
        else if(status == SocketConnectFailed) {
            auto evt = event::EzyConnectionFailureEvent::create(mConnectionFailedReason);
            this->mSocketEventQueue->addEvent(evt);
            break;
        }
        else if(status == SocketDisconnected) {
            auto event = event::EzyDisconnectionEvent::create(mDisconnectReason);
            mSocketEventQueue->addEvent(event);
            break;
        }
    }
    mLocalSocketStatuses.clear();
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
    auto status = mSocketStatuses->last();
    if(status == SocketConnected) {
        if(mSocketReader->isActive()) {
            processReceivedMessages0();
        }
    }
    auto statusLast = mSocketStatuses->last();
    if(isSocketDisconnectable(statusLast)) {
        if(mSocketReader->isStopped()) {
            onDisconnected(mDisconnectReason);
        }
        else if(mSocketWriter->isStopped()) {
            onDisconnected(mDisconnectReason);
        }
    }
}

void EzySocketClient::processReceivedMessages0() {
    mPingManager->setLostPingCount(0);
    popReadMessages();
    for (int i = 0 ; i < mLocalMessageQueue.size() ; ++i) {
        processReceivedMessage(mLocalMessageQueue[i]);
    }
    mLocalMessageQueue.clear();
}

void EzySocketClient::popReadMessages() {
    mSocketReader->popMessages(mLocalMessageQueue);
}

void EzySocketClient::processReceivedMessage(EzySocketData* message) {
    auto array = (entity::EzyArray*)message;
    auto cmdId = array->getInt(0);
    auto data = array->getArray(1, 0);
    auto cmd = (constant::EzyCommand)cmdId;
    printReceivedData(cmd, data);
    if(cmd == constant::Disconnect) {
        auto reasonId = data->getInt(0);
        mDisconnectReason = (int)reasonId;
        mSocketStatuses->push(SocketDisconnecting);
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
