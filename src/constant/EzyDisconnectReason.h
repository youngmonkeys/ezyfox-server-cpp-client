#pragma once

#include <string>
#include "../EzyMacro.h"

EZY_NAMESPACE_START_WITH_ONLY(constant)

enum EzyDisconnectReason
{
    UnknownDisconnection = 0,
    Idle = 1,
    NotLoggedIn = 2,
    AnotherSessionLogin = 3,
    AdminBan = 4,
    AdminKick = 5,
    MaxRequestPerSecond = 6,
    MaxRequestSize = 7,
    ServerError = 8,
    ServerNotResponding = 400
};

std::string getDisconnectReasonName(int reason);

EZY_NAMESPACE_END_WITH
