#pragma once

#include "../EzyMacro.h"

EZY_NAMESPACE_START_WITH_ONLY(event)
    
enum EzyEventType {
    ConnectionSuccess = 1,
    ConnectionFailure = 2,
    Disconnection = 3,
    LostPing = 4,
    TryConnect = 5
};

EZY_NAMESPACE_END_WITH
