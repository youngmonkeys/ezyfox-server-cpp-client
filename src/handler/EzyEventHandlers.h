#pragma once

#include <map>
#include "../EzyMacro.h"
#include "EzyEvent.h"
#include "EzyEventHandler.h"

EZY_NAMESPACE_START
namespace handler {

class EzyEventHandlers {
protected:
    std::map<event::EzyEventType, EzyEventHandler<event::EzyEvent>*> handlers;
public:
    EzyEventHandlers();
    ~EzyEventHandlers();
    
    void handleEvent(event::EzyEvent* event);
    
    template<class T>
    void addEventHandler(event::EzyEventType eventType, EzyEventHandler<T>* handler);
};
    
template<class T>
void EzyEventHandlers::addEventHandler(event::EzyEventType eventType, EzyEventHandler<T> *handler) {
    handlers[eventType] = (EzyEventHandler<event::EzyEvent>*)handler;
}

}
EZY_NAMESPACE_END
