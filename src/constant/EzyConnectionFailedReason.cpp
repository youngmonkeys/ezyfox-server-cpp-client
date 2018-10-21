#include <map>
#include "EzyConnectionFailedReason.h"

EZY_NAMESPACE_START_WITH_ONLY(constant)

static std::map<int, std::string> sConnectionFailedReasonNames = {
    {TimeOut, "TimeOut"},
    {NetworkUnreachable, "NetworkUnreachable"},
    {UnknownHost, "UnknownHost"},
    {ConnectionRefused, "ConnectionRefused"},
    {UnknownFailure, "UnknownFailure"}
};

std::string getConnectionFailedReasonName(int reason) {
    if(sConnectionFailedReasonNames.count(reason) == 0)
        return std::to_string(reason);
    return sConnectionFailedReasonNames[reason];
}

EZY_NAMESPACE_END_WITH
