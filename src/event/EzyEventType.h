#pragma once

#include <string>
#include "../EzyMacro.h"

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
