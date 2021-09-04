#include <chrono>
#include "EzySender.h"
#include "EzyPingSchedule.h"
#include "EzySocketCore.h"
#include "EzySocketClient.h"
#include "../EzyClient.h"
#include "../event/EzyEvent.h"
#include "../logger/EzyLogger.h"
#include "../manager/EzyPingManager.h"
#include "../constant/EzyDisconnectReason.h"
#include "../concurrent/EzyScheduleAtFixedRate.h"

EZY_NAMESPACE_START_WITH(socket)

EzyPingSchedule::EzyPingSchedule(EzyClient* client) {
    this->mSchedule = 0;
    this->mClient = client;
    this->mSocketEventQueue = 0;
    this->mPingRequest = new request::EzyPingRequest();
    this->mPingManager = client->getPingManager();
}

EzyPingSchedule::~EzyPingSchedule() {
    if(mSchedule) mSchedule->stop();
    this->mSchedule = 0;
    this->mClient = 0;
    this->mPingManager = 0;
    this->mSocketEventQueue = 0;
    EZY_SAFE_DELETE(mPingRequest);
}

void EzyPingSchedule::start() {
    std::unique_lock<std::mutex> lock(mPingMutex);
    if(mSchedule) mSchedule->stop();
    mSchedule = new concurrent::EzyScheduleAtFixedRate("ezyfox-ping-schedule");
    auto period = mPingManager->getPingPeriod();
    mSchedule->schedule([=]() {this->sendPingRequest();}, period, period);
}

void EzyPingSchedule::stop() {
    std::unique_lock<std::mutex> lock(mPingMutex);
    if(mSchedule)
        mSchedule->stop();
    mSchedule = 0;
}

void EzyPingSchedule::sendPingRequest() {
    auto lostPingCount = mPingManager->increaseLostPingCount();
    auto maxLostPingCount = mPingManager->getMaxLostPingCount();
    if(lostPingCount >= maxLostPingCount) {
        mClient->getSocket()->disconnect(constant::ServerNotResponding);
    }
    else {
        mClient->send(mPingRequest);
    }
    if(lostPingCount > 1) {
        logger::log("lost ping count: %d", lostPingCount);
        auto event = event::EzyLostPingEvent::create(lostPingCount);
        mSocketEventQueue->addEvent(event);
    }
}

EZY_NAMESPACE_END_WITH

