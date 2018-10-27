#include <chrono>
#include "EzySender.h"
#include "EzyPingSchedule.h"
#include "EzySocketCore.h"
#include "../EzyClient.h"
#include "../manager/EzyPingManager.h"
#include "../event/EzyEvent.h"
#include "../constant/EzyDisconnectReason.h"

EZY_NAMESPACE_START_WITH(socket)

EzyPingSchedule::EzyPingSchedule(EzyClient* client) {
    this->mActive = false;
    this->mThread = 0;
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
    auto currentThread = mThread;
    mThread = new std::thread(&EzyPingSchedule::loop, this);
    mThread->detach();
    EZY_SAFE_DELETE(currentThread);
}

void EzyPingSchedule::stop() {
    mActive = false;
    EZY_SAFE_DELETE(mThread);
}

void EzyPingSchedule::loop() {
    mActive = true;
    while (mActive) {
        auto maxLostPingCount = mPingManager->getMaxLostPingCount();
        auto lostPingCount = mPingManager->increaseLostPingCount();
        if(lostPingCount <= maxLostPingCount) {
            sendPingRequest();
            auto period = mPingManager->getPingPeriod();
            std::this_thread::sleep_for(std::chrono::milliseconds(period));
        }
        else {
            mActive = false;
            auto event = event::EzyDisconnectionEvent::create(constant::ServerNotResponding);
            mSocketEventQueue->addEvent(event);
            break;
        }
        if(lostPingCount > 1) {
            auto event = event::EzyLostPingEvent::create(lostPingCount);
            mSocketEventQueue->addEvent(event);
        }
    }
}

void EzyPingSchedule::sendPingRequest() {
    auto request = request::EzyPingRequest::create();
//    mClient->send(request);
}

EZY_NAMESPACE_END_WITH

