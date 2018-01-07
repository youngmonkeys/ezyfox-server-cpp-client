#pragma once

#include "EzyEventType.h"
#include "../entity/EzyValue.h"

EZY_NAMESPACE_START
namespace event {

class EzyEvent : public entity::EzyRef {
public:
    virtual EzyEventType getType() = 0;
protected:
    EZY_SYNTHESIZE_READONLY(entity::EzyValue*, Args)
};
    
class EzyConnectionSuccessEvent : public EzyEvent {
public:
    static EzyConnectionSuccessEvent* create();
    EzyEventType getType();
};

class EzyHandshakeEvent : public EzyEvent {
public:
    static EzyHandshakeEvent* create();
    EzyEventType getType();
};

}
EZY_NAMESPACE_END
