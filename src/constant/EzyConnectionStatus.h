#pragma once

#include <string>
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

std::string getConnectionStatusName(int command);

bool isClientConnectable(EzyConnectionStatus status);
bool isClientReconnectable(EzyConnectionStatus status);
bool isClientDestroyable(EzyConnectionStatus status);

EZY_NAMESPACE_END_WITH
