#pragma once

#include "../EzyMacro.h"
#include "../base/EzyRef.h"
#include "EzyEventType.h"
#include "../entity/EzyValue.h"

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

EZY_DECLARE_EVENT_CLASS(ConnectionClosed)

//===================================

class EzyConnectionFailureEvent : public EzyEvent {
protected:
    EZY_SYNTHESIZE_READONLY(int, Reason);
public:
    static EzyConnectionFailureEvent* create(int reason);
    EzyConnectionFailureEvent(int reason);
    EzyEventType getType();
};

//===================================

class EzyDisconnectionEvent : public EzyEvent {
protected:
    EZY_SYNTHESIZE_READONLY(int, Reason);
public:
    static EzyDisconnectionEvent* create(int reason);
    EzyDisconnectionEvent(int reason);
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
