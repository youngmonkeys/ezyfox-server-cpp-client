#include "EzyEventHandlers.h"

#define HANDLERS_MAP std::map<event::EzyEventType, EzyEventHandler<event::EzyEvent>*>

EZY_NAMESPACE_START_WITH(handler)
    
EzyEventHandlers::EzyEventHandlers() {
}

EzyEventHandlers::~EzyEventHandlers() {
    for(auto it = handlers.begin() ; it != handlers.end() ; ++it) {
        EZY_SAFE_DELETE(it->second);
    }
    handlers.clear();
}
    
void EzyEventHandlers::handleEvent(event::EzyEvent *event) {
    auto eventType = event->getType();
    auto *hanlder = handlers.at(eventType);
    if(hanlder) hanlder->handle(event);
}

EZY_NAMESPACE_END_WITH
