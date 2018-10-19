#pragma once

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

EZY_NAMESPACE_END_WITH
