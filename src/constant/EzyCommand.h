#pragma once

#include "../EzyMacro.h"

EZY_NAMESPACE_START
namespace constant {

enum EzyCommand {
    error = 10,
    handshake = 11,
    ping = 12,
    pong = 13,
    disconnect = 14,
    pluginRequest = 15,
    login = 20,
    loginError = 21,
    logout = 22,
    appAccess = 30,
    appRequest = 31,
    appJoined = 32,
    appExit = 33,
    appAccessError = 34
};

}
EZY_NAMESPACE_END
