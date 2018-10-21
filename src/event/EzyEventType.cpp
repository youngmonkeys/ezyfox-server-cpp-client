#include <map>
#include "EzyEventType.h"

EZY_NAMESPACE_START_WITH_ONLY(event)

static std::map<int, std::string> sEventTypeNames = {
    {ConnectionSuccess, "ConnectionSuccess"},
    {ConnectionFailure, "ConnectionFailure"},
    {ConnectionClosed, "ConnectionClosed"},
    {Disconnection, "Disconnection"},
    {LostPing, "LostPing"},
    {TryConnect, "TryConnect"}
};

std::string getEventTypeName(int eventType) {
    if(sEventTypeNames.count(eventType) == 0)
        return std::to_string(eventType);
    return sEventTypeNames[eventType];
}

EZY_NAMESPACE_END_WITH
