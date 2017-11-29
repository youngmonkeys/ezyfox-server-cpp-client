#include <thread>
#include "EzySocketClient.h"
#include "../pool/EzyAutoReleasePool.h"

EZY_NAMESPACE_START
namespace socket {

EzySocketClient::EzySocketClient() {
    mPort = 0;
    mHost = "";
    mConnectTime = 0;
    mSocketReader = 0;
    mSocketWriter = 0;
    mDataHandler = nullptr;
    mStatusHandler = nullptr;
    mReleasePool = 0;
}

EzySocketClient::~EzySocketClient() {
    this->closeSocket();
    this->clearAdapter();
    EZY_DELETE_0(mDataHandler)
    EZY_DELETE_0(mStatusHandler);
}

void EzySocketClient::closeSocket() {
    
}

void EzySocketClient::createAdapter() {
    
}

bool EzySocketClient::connectThread() {
    return false;
}

void EzySocketClient::connectTo(const std::string& host, int port){
    clearAdapter();
    createAdapter();
    closeSocket();
    
    mClientStatus.clear();
    this->setStatus(EzySocketStatusType::Connecting);
    this->mHost = host;
    this->mPort = port;
    
    this->retain();
    std::thread newThread(&EzySocketClient::updateConnection, this);
    newThread.detach();
}

void EzySocketClient::updateConnection(){
    auto currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    auto dt = currentTime - mConnectTime;
    if (dt < 2000){ //delay 2000ms
        std::this_thread::sleep_for(std::chrono::milliseconds(2000 - dt));
    }
    
    bool b = this->connectThread();
    mConnectTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    
    if (b){
        this->startAdapter();
        this->setStatus(EzySocketStatusType::Connected);
    }
    else{
        this->resetSocket();
        if (this->getStatus() == EzySocketStatusType::Connecting){
            this->setStatus(EzySocketStatusType::ConnectFailure);
        }
    }
    
    pool::EzyAutoReleasePool::getInstance()->removePool();
    this->release();
}

void EzySocketClient::startAdapter(){
    
}

void EzySocketClient::closeClient(){
    if (mSocketWriter) {
        mSocketWriter->stop();
    }
    if(mSocketReader) {
        mSocketReader->stop();
    }
    
    mClientStatus.clear();
    this->setStatus(EzySocketStatusType::Closed);
    closeSocket();
}

EzySocketStatusType EzySocketClient::getStatus(){
    return mClientStatus.get();
}

void EzySocketClient::setStatus(EzySocketStatusType status, bool event){
    mClientStatus.set(status, event);
}

void EzySocketClient::sendMessage(EzySocketData* data){
    if (mSocketWriter){
        mSocketWriter->pushMessage(data);
    }
}

void EzySocketClient::processEvent(){
    if (mStatusHandler){
        mClientStatus.popAllStatus(mStatusBuffer);
        for (int i = 0; i < mStatusBuffer.size(); i++){
            mStatusHandler->handleSocketStatus(mStatusBuffer[i]);
        }
        mStatusBuffer.clear();
    }
}

void EzySocketClient::processRecvMessage(){
    if (mDataHandler){
        auto data = mSocketReader->popMessage();
        while (data){
            if (this->getStatus() != EzySocketStatusType::Connected){
                break;
            }
            mDataHandler->handleSocketData(data);
            data = mSocketReader->popMessage();
        }
    }
}

void EzySocketClient::clearAdapter(){
    std::unique_lock<std::mutex> lk(mClientMutex);
    if (mSocketReader){
        mSocketReader->stop();
        mSocketReader->release();
        mSocketReader = 0;
    }
    if (mSocketWriter){
        mSocketWriter->stop();
        mSocketWriter->release();
        mSocketWriter = 0;
    }
}

void EzySocketClient::resetSocket(){
    
}

void EzySocketClient::processSocketError(){
    if (this->getStatus() == EzySocketStatusType::Connected){
        this->setStatus(EzySocketStatusType::LostConnection);
    }
}

void EzySocketClient::processMessage(){
    if (this->getStatus() == EzySocketStatusType::Connected){
        if (mSocketReader && mSocketWriter){
            processRecvMessage();
            if (!mSocketReader->isRunning()){
                if (this->getStatus() == EzySocketStatusType::Connected){
                    this->processSocketError();
                    this->closeSocket();
                    this->clearAdapter();
                }
            }
        }
    }
    
    processEvent();
    
    if (!mReleasePool){
        mReleasePool = pool::EzyAutoReleasePool::getInstance()->getPool();
    }
    mReleasePool->releaseAll();
}

void EzySocketClient::setDataHandler(EzySocketDataHandler *handler) {
    this->mDataHandler = handler;
}

void EzySocketClient::setStatusHandler(EzySocketStatusHandler *handler) {
    this->mStatusHandler = handler;
}

}
EZY_NAMESPACE_END
