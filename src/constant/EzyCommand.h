#pragma once

#include "../EzyMacro.h"

EZY_NAMESPACE_START_WITH(constant)

enum EzyCommand {
    Error = 10,
    Handshake = 11,
    Ping = 12,
    Pong = 13,
    Disconnect = 14,
    Login = 20,
    LoginError = 21,
    AppAccess = 30,
    AppRequest = 31,
    AppExit = 33,
    AppAccessError = 34,
    PluginInfo = 40,
    PluginRequestByName = 41,
    PluginRequestById = 42
};

EZY_NAMESPACE_END_WITH
