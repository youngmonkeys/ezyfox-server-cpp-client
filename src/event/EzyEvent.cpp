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
    var pRet = new EzyConnectionFailureEvent(reason);
    pRet->autorelease();
    return pRet;
}

EzyConnectionFailureEvent::EzyConnectionFailureEvent(constant::EzyConnectionFailedReason reason) {
    this->reason = reason;
}

//===============================================

EzyDisconnectionEvent* EzyDisconnectionEvent::create(constant::EzyDisconnectReason reason) {
    var pRet = new EzyDisconnectionEvent(reason);
    pRet->autorelease();
    return pRet;
}

EzyDisconnectionEvent::EzyDisconnectionEvent(constant::EzyDisconnectReason reason) {
    this->reason = reason;
}
    
//===============================================

EzyLostPingEvent* EzyLostPingEvent::create(int count) {
    var pRet = new EzyLostPingEvent(count);
    pRet->autorelease();
    return pRet;
}

EzyLostPingEvent::EzyLostPingEvent(int count) {
    this->count = count;
}

//===============================================

EzyTryConnectEvent* EzyTryConnectEvent::create(int count) {
    var pRet = new EzyTryConnectEvent(count);
    pRet->autorelease();
    return pRet;
}

EzyTryConnectEvent::EzyTryConnectEvent(int count) {
    this->count = count;
}

EZY_NAMESPACE_END
