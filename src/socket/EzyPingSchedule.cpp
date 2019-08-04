#include <chrono>
#include "EzySender.h"
#include "EzyPingSchedule.h"
#include "EzySocketCore.h"
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
    this->mPingManager = client->getPingManager();
}

EzyPingSchedule::~EzyPingSchedule() {
    stop();
    this->mClient = 0;
    this->mPingManager = 0;
    this->mSocketEventQueue = 0;
}

void EzyPingSchedule::start() {
    std::unique_lock<std::mutex> lock(mMutex);
    auto currentSchedule = mSchedule;
    mSchedule = new concurrent::EzyScheduleAtFixedRate();
    auto period = mPingManager->getPingPeriod();
    mSchedule->schedule([this]() {this->sendPingRequest();}, period, period);
    EZY_SAFE_DELETE(currentSchedule);
}

void EzyPingSchedule::stop() {
    EZY_SAFE_DELETE(mSchedule);
}

void EzyPingSchedule::sendPingRequest() {
    auto lostPingCount = mPingManager->increaseLostPingCount();
    auto maxLostPingCount = mPingManager->getMaxLostPingCount();
    if(lostPingCount >= maxLostPingCount) {
        auto event = event::EzyDisconnectionEvent::create(constant::ServerNotResponding);
        mSocketEventQueue->addEvent(event);
    }
    else {
        auto request = request::EzyPingRequest::create();
        mClient->send(request);
    }
    if(lostPingCount > 1) {
        logger::log("lost ping count: %d", lostPingCount);
        auto event = event::EzyLostPingEvent::create(lostPingCount);
        mSocketEventQueue->addEvent(event);
    }
}

EZY_NAMESPACE_END_WITH

