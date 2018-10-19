#include "../logger/EzyLogger.h"
#include "EzyEventHandlers.h"

EZY_NAMESPACE_START_WITH(handler)
    
EzyEventHandlers::EzyEventHandlers() {
}

EzyEventHandlers::~EzyEventHandlers() {
    for(auto it = handlers.begin() ; it != handlers.end() ; ++it) {
        EZY_SAFE_DELETE(it->second);
    }
    handlers.clear();
}
    
void EzyEventHandlers::handle(event::EzyEvent *event) {
    auto eventType = event->getType();
    auto hanlder = handlers[eventType];
    if(hanlder)
        hanlder->handle(event);
    else
        logger::log("has no handler for event type: %d", eventType);
}

void EzyEventHandlers::addHandler(event::EzyEventType eventType, EzyEventHandler *handler) {
    handlers[eventType] = handler;
}

EZY_NAMESPACE_END_WITH
