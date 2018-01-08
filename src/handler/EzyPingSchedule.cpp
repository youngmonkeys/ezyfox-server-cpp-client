#include <chrono>
#include "EzyPingSchedule.h"

EZY_NAMESPACE_START
namespace handler {
    
EzyPingSchedule::EzyPingSchedule(request::EzyRequestDeliver* requestDeliver) {
    mActive = false;
    mThread = nullptr;
    mRequestDeliver = requestDeliver;
}
    
EzyPingSchedule::~EzyPingSchedule() {
    stop();
}
    
void EzyPingSchedule::start() {
    auto currentThread = mThread;
    mThread = new std::thread(&EzyPingSchedule::loopSendPingRequest, this);
    mThread->detach();
    EZY_SAFE_DELETE(currentThread);
}
    
void EzyPingSchedule::stop() {
    mActive = false;
    EZY_SAFE_DELETE(mThread);
}
    
void EzyPingSchedule::loopSendPingRequest() {
    mActive = true;
    while (mActive) {
        sendPingRequest();
        std::this_thread::sleep_for(std::chrono::seconds(mPeriod));
    }
}
    
void EzyPingSchedule::sendPingRequest() {
    auto request = request::EzyPingRequest::create();
    mRequestDeliver->send(request);
}
    
}
EZY_NAMESPACE_END
