#pragma once

#include "../EzyMacro.h"

EZY_NAMESPACE_START_WITH_ONLY(constant)

enum EzyConnectionStatus
{
    Null = 0,
    Connecting = 1,
    Connected = 2,
    Disconnected = 3,
    Failure = 4,
    Reconnecting = 5
};

EZY_NAMESPACE_END_WITH
