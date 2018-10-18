#pragma once

#include "../EzyMacro.h"

EZY_NAMESPACE_START_WITH(constant)

enum EzyDisconnectReason
{
    Uknown = 0,
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

EZY_NAMESPACE_END_WITH
