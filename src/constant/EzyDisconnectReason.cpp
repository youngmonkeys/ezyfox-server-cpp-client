#include <map>
#include "EzyDisconnectReason.h"

EZY_NAMESPACE_START_WITH_ONLY(constant)

static std::map<int, std::string> sDisconnectReasonNames = {
    {UnknownDisconnection, "UnknownDisconnection"},
    {Idle, "Idle"},
    {NotLoggedIn, "NotLoggedIn"},
    {AnotherSessionLogin, "AnotherSessionLogin"},
    {AdminBan, "AdminBan"},
    {AdminKick, "AdminKick"},
    {MaxRequestPerSecond, "MaxRequestPerSecond"},
    {MaxRequestSize, "MaxRequestSize"},
    {ServerError, "ServerError"},
    {ServerNotResponding, "ServerNotResponding"}
};

std::string getDisconnectReasonName(int reason) {
    auto it = sDisconnectReasonNames.find(reason);
    if(it != sDisconnectReasonNames.end())
        return it->second;
    return std::to_string(reason);
}

EZY_NAMESPACE_END_WITH
