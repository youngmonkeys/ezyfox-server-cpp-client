#include <map>
#include "EzyConnectionStatus.h"

EZY_NAMESPACE_START_WITH_ONLY(constant)

static std::map<int, std::string> sConnectionStatusNames = {
    {Null, "Null"},
    {Connecting, "Connecting"},
    {Connected, "Connected"},
    {Disconnected, "Disconnected"},
    {Failure, "Failure"},
    {Reconnecting, "Reconnecting"}
};

std::string getConnectionStatusName(int command) {
    auto it = sConnectionStatusNames.find(command);
    if(it != sConnectionStatusNames.end())
        return it->second;
    return std::to_string(command);
}

bool isClientConnectable(EzyConnectionStatus status) {
    return status == Null || status == Disconnected || status == Failure;
}

bool isClientReconnectable(EzyConnectionStatus status) {
    return status == Disconnected || status == Failure;
}

bool isClientDestroyable(EzyConnectionStatus status) {
    return status == Null || status == Disconnected || status == Failure;
}

EZY_NAMESPACE_END_WITH
