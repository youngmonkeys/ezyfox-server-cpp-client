#include <chrono>
#include "EzySender.h"
#include "EzyPingSchedule.h"
#include "../manager/EzyPingManager.h"
#include "../EzyClient.h"

EZY_NAMESPACE_START_WITH(socket)

EzyPingSchedule::EzyPingSchedule(EzyClient* client) {
    this->mActive = false;
    this->mThread = nullptr;
    this->mClient = client;
    this->mPingManager = client->getPingManager();
}

EzyPingSchedule::~EzyPingSchedule() {
    stop();
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
        sendPingRequest();
        auto period = mPingManager->getPingPeriod();
        std::this_thread::sleep_for(std::chrono::seconds(period));
    }
}

void EzyPingSchedule::sendPingRequest() {
    auto request = request::EzyPingRequest::create();
    mClient->send(request);
}

EZY_NAMESPACE_END_WITH

