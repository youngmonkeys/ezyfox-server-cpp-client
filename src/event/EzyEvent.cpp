#include "EzyEvent.h"

#define EZY_IMPLEMENT_EVENT_CLASS(className, eventType)\
Ezy##className##Event* Ezy##className##Event::create() {\
    auto pRet = new Ezy##className##Event();\
    return pRet;\
}\
EzyEventType Ezy##className##Event::getType() {\
    return eventType;\
}

EZY_NAMESPACE_START_WITH(event)
//===============================================
    
EZY_IMPLEMENT_EVENT_CLASS(ConnectionSuccess, EzyEventType::ConnectionSuccess);

//===============================================

EZY_IMPLEMENT_EVENT_CLASS(ConnectionClosed, EzyEventType::ConnectionClosed);
    
//===============================================

EzyConnectionFailureEvent* EzyConnectionFailureEvent::create(int reason) {
    auto pRet = new EzyConnectionFailureEvent(reason);
    return pRet;
}

EzyConnectionFailureEvent::EzyConnectionFailureEvent(int reason) {
    this->mReason = reason;
}

EzyEventType EzyConnectionFailureEvent::getType() {
    return EzyEventType::ConnectionFailure;
}

//===============================================

EzyDisconnectionEvent* EzyDisconnectionEvent::create(int reason) {
    auto pRet = new EzyDisconnectionEvent(reason);
    return pRet;
}

EzyDisconnectionEvent::EzyDisconnectionEvent(int reason) {
    this->mReason = reason;
}

EzyEventType EzyDisconnectionEvent::getType() {
    return EzyEventType::Disconnection;
}
    
//===============================================

EzyLostPingEvent* EzyLostPingEvent::create(int count) {
    auto pRet = new EzyLostPingEvent(count);
    return pRet;
}

EzyLostPingEvent::EzyLostPingEvent(int count) {
    this->mCount = count;
}

EzyEventType EzyLostPingEvent::getType() {
    return EzyEventType::LostPing;
}

//===============================================

EzyTryConnectEvent* EzyTryConnectEvent::create(int count) {
    auto pRet = new EzyTryConnectEvent(count);
    return pRet;
}

EzyTryConnectEvent::EzyTryConnectEvent(int count) {
    this->mCount = count;
}

EzyEventType EzyTryConnectEvent::getType() {
    return EzyEventType::TryConnect;
}

EZY_NAMESPACE_END_WITH
