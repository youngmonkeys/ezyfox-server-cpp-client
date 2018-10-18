#pragma once

#include "../EzyMacro.h"

EZY_NAMESPACE_START_WITH(constant)

enum EzyConnectionFailedReason {
    TimeOut = 0,
    NetworkUnreachable = 1,
    UnknownHost = 2,
    ConnectionRefused = 3,
    Unknown = 4
};

EZY_NAMESPACE_END_WITH
