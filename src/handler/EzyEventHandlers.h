#pragma once

#include <map>
#include "../EzyMacro.h"
#include "../event/EzyEvent.h"
#include "../event/EzyEventType.h"
#include "EzyEventHandler.h"

EZY_NAMESPACE_START_WITH(handler)

class EzyEventHandlers {
protected:
    std::map<event::EzyEventType, EzyEventHandler*> handlers;
public:
    EzyEventHandlers();
    ~EzyEventHandlers();
    void handle(event::EzyEvent* event);
    void addHandler(event::EzyEventType eventType, EzyEventHandler* handler);
};

EZY_NAMESPACE_END_WITH
