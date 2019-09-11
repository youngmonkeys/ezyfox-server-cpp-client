#ifndef EzyEventType_h
#define EzyEventType_h

#include <string>
#include "../EzyMacro.h"

#define NUMBER_OF_EVENTS 6

EZY_NAMESPACE_START_WITH_ONLY(event)
    
enum EzyEventType {
    ConnectionSuccess = 1,
    ConnectionFailure = 2,
    ConnectionClosed = 3,
    Disconnection = 4,
    LostPing = 5,
    TryConnect = 6
};

std::string getEventTypeName(int eventType);

EZY_NAMESPACE_END_WITH

#endif /* EzyEventType_h */
