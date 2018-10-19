#include <thread>
#include "EzySocketClient.h"
#include "../event/EzyEvent.h"
#include "../gc/EzyAutoReleasePool.h"
#include "../manager/EzyHandlerManager.h"
#include "../handler/EzyEventHandlers.h"
#include "../handler/EzyDataHandlers.h"
#include "../constant/EzyDisconnectReason.h"

EZY_NAMESPACE_START_WITH(socket)

EzySocketClient::EzySocketClient() {
    mPort = 0;
    mHost = "";
    mConnectTime = 0;
    mSocketReader = 0;
    mSocketWriter = 0;
    mReleasePool = 0;
}

EzySocketClient::~EzySocketClient() {
    this->closeSocket();
    this->clearAdapter();
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

void EzySocketClient::connectTo(const std::string& host, int port){
    clearAdapter();
    createAdapter();
    closeSocket();
    mSocketEventQueue.clear();
    mHost = host;
    mPort = port;
    retain();
    std::thread newThread(&EzySocketClient::updateConnection, this);
    newThread.detach();
}

void EzySocketClient::updateConnection() {
    auto currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    auto dt = currentTime - mConnectTime;
    if (dt < 2000){ //delay 2000ms
        std::this_thread::sleep_for(std::chrono::milliseconds(2000 - dt));
    }
    
    bool success = this->connectNow();
    mConnectTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    
    if (success){
        this->startAdapter();
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
    mSocketEventQueue.popAll(mStatusBuffer);
    for (int i = 0; i < mStatusBuffer.size(); i++) {
        mEventHandlers->handle(mStatusBuffer[i]);
    }
    mStatusBuffer.clear();
}

void EzySocketClient::processRecvMessage() {
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
    processRecvMessage();
    mReleasePool = gc::EzyAutoReleasePool::getInstance()->getPool();
    mReleasePool->releaseAll();
}

EZY_NAMESPACE_END_WITH
