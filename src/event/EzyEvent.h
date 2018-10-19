#pragma once

#include "../EzyMacro.h"
#include "../base/EzyRef.h"
#include "EzyEventType.h"
#include "../entity/EzyValue.h"
#include "../constant/EzyDisconnectReason.h"
#include "../constant/EzyConnectionFailedReason.h"

#define EZY_DECLARE_EVENT_CLASS(className)\
class Ezy##className##Event : public EzyEvent {\
public:\
    static Ezy##className##Event* create();\
    EzyEventType getType();\
};

EZY_NAMESPACE_START_WITH(event)

//===================================
    
class EzyEvent : public base::EzyRef {
public:
    virtual EzyEventType getType() = 0;
};
    
//===================================
    
EZY_DECLARE_EVENT_CLASS(ConnectionSuccess)
    
//===================================

class EzyConnectionFailureEvent : public EzyEvent {
protected:
    EZY_SYNTHESIZE_READONLY(constant::EzyConnectionFailedReason, Reason);
public:
    static EzyConnectionFailureEvent* create(constant::EzyConnectionFailedReason reason);
    EzyConnectionFailureEvent(constant::EzyConnectionFailedReason reason);
    EzyEventType getType();
};

//===================================

class EzyDisconnectionEvent : public EzyEvent {
protected:
    EZY_SYNTHESIZE_READONLY(constant::EzyDisconnectReason, Reason);
public:
    static EzyDisconnectionEvent* create(constant::EzyDisconnectReason reason);
    EzyDisconnectionEvent(constant::EzyDisconnectReason reason);
    EzyEventType getType();
};

//===================================

class EzyLostPingEvent : public EzyEvent {
protected:
    EZY_SYNTHESIZE_READONLY(int, Count);
public:
    static EzyLostPingEvent* create(int count);
    EzyLostPingEvent(int count);
    EzyEventType getType();
};

//===================================

class EzyTryConnectEvent : public EzyEvent {
protected:
    EZY_SYNTHESIZE_READONLY(int, Count);
public:
    static EzyTryConnectEvent* create(int count);
    EzyTryConnectEvent(int count);
    EzyEventType getType();
};

EZY_NAMESPACE_END_WITH
