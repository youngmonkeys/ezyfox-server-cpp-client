#ifndef EzyEventHandlers_h
#define EzyEventHandlers_h

#include <map>
#include "../EzyMacro.h"
#include "../event/EzyEvent.h"
#include "../event/EzyEventType.h"

EZY_NAMESPACE_START
class EzyClient;
EZY_NAMESPACE_END

EZY_NAMESPACE_START_WITH(handler)

class EzyEventHandler;

class EzyEventHandlers {
protected:
    EzyClient* mClient;
    std::map<event::EzyEventType, EzyEventHandler*> mHandlers;
public:
    EzyEventHandlers(EzyClient* mClient);
    ~EzyEventHandlers();
    void handle(event::EzyEvent* event);
    EzyEventHandler* getHandler(event::EzyEventType eventType);
    void addHandler(event::EzyEventType eventType, EzyEventHandler* handler);
};

EZY_NAMESPACE_END_WITH

#endif /* EzyEventHandlers_h */
