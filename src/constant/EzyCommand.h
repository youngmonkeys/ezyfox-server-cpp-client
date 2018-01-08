#pragma once

#include "../EzyMacro.h"

EZY_NAMESPACE_START
namespace command {

enum EzyCommand {
    Error = 10,
    Handshake = 11,
    Ping = 12,
    Pong = 13,
    Disconnect = 14,
    PluginRequest = 15,
    Login = 20,
    LoginError = 21,
    Logout = 22,
    AppAccess = 30,
    AppRequest = 31,
    AppJoined = 32,
    AppExit = 33,
    AppAccessError = 34
};

}
EZY_NAMESPACE_END
