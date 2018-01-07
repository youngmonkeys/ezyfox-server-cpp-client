#include "EzyEvent.h"

EZY_NAMESPACE_START
namespace event {
 
EzyConnectionSuccessEvent* EzyConnectionSuccessEvent::create() {
    EzyConnectionSuccessEvent* pRet = new EzyConnectionSuccessEvent();
    pRet->autorelease();
    return pRet;
}
    
EzyEventType EzyConnectionSuccessEvent::getType() {
    return EzyEventType::connectionSuccess;
}
    
//========================

EzyHandshakeEvent* EzyHandshakeEvent::create() {
    EzyHandshakeEvent* pRet = new EzyHandshakeEvent();
    pRet->autorelease();
    return pRet;
}
    
EzyEventType EzyHandshakeEvent::getType() {
    return EzyEventType::handshake;
}
    
}
EZY_NAMESPACE_END
