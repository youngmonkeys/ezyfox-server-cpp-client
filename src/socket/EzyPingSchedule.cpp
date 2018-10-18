#include <chrono>
#include "EzySender.h"
#include "../manager/EzyPingManager.h"
#include "EzyPingSchedule.h"

EZY_NAMESPACE_START_WITH(socket)

EzyPingSchedule::EzyPingSchedule(EzySender* sender, manager::EzyPingManager* manager) {
    this->mActive = false;
    this->mThread = nullptr;
    this->mSender = sender;
    this->mPingManager = manager;
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
    mSender->send(request);
}

EZY_NAMESPACE_END_WITH

