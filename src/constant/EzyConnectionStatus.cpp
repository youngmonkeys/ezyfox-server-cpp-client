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
    auto name = sConnectionStatusNames[command];
    return name;
}

EZY_NAMESPACE_END_WITH
