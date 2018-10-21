#pragma once

#include <string>
#include "../EzyMacro.h"

EZY_NAMESPACE_START_WITH_ONLY(constant)

enum EzyConnectionFailedReason {
    TimeOut = 0,
    NetworkUnreachable = 1,
    UnknownHost = 2,
    ConnectionRefused = 3,
    UnknownFailure = 4
};

std::string getConnectionFailedReasonName(int reason);

EZY_NAMESPACE_END_WITH
