#include "EzyEvent.h"

#define EZY_IMPLEMENT_EVENT_CLASS(className, eventType)\
Ezy##className##Event* Ezy##className##Event::create() {\
    Ezy##className##Event* pRet = new Ezy##className##Event();\
    pRet->autorelease();\
    return pRet;\
}\
EzyEventType Ezy##className##Event::getType() {\
    return eventType;\
}

EZY_NAMESPACE_START_WITH(event)
//===============================================
    
EZY_IMPLEMENT_EVENT_CLASS(ConnectionSuccess, EzyEventType::ConnectionSuccess);
    
//===============================================

EzyConnectionFailureEvent* EzyConnectionFailureEvent::create(constant::EzyConnectionFailedReason reason) {
    auto pRet = new EzyConnectionFailureEvent(reason);
    pRet->autorelease();
    return pRet;
}

EzyConnectionFailureEvent::EzyConnectionFailureEvent(constant::EzyConnectionFailedReason reason) {
    this->mReason = reason;
}

EzyEventType EzyConnectionFailureEvent::getType() {
    return EzyEventType::ConnectionFailure;
}

//===============================================

EzyDisconnectionEvent* EzyDisconnectionEvent::create(constant::EzyDisconnectReason reason) {
    auto pRet = new EzyDisconnectionEvent(reason);
    pRet->autorelease();
    return pRet;
}

EzyDisconnectionEvent::EzyDisconnectionEvent(constant::EzyDisconnectReason reason) {
    this->mReason = reason;
}

EzyEventType EzyDisconnectionEvent::getType() {
    return EzyEventType::Disconnection;
}
    
//===============================================

EzyLostPingEvent* EzyLostPingEvent::create(int count) {
    auto pRet = new EzyLostPingEvent(count);
    pRet->autorelease();
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
    pRet->autorelease();
    return pRet;
}

EzyTryConnectEvent::EzyTryConnectEvent(int count) {
    this->mCount = count;
}

EzyEventType EzyTryConnectEvent::getType() {
    return EzyEventType::TryConnect;
}

EZY_NAMESPACE_END_WITH
