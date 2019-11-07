#include <map>
#include "EzyCommand.h"

EZY_NAMESPACE_START_WITH_ONLY(constant)

static std::map<int, std::string> sCommandNames = {
    {Error, "Error"},
    {Handshake, "Handshake"},
    {Ping, "Ping"},
    {Pong, "Pong"},
    {Disconnect, "Disconnect"},
    {Login, "Login"},
    {LoginError, "LoginError"},
    {AppAccess, "AppAccess"},
    {AppRequest, "AppRequest"},
    {AppExit, "AppExit"},
    {AppAccessError, "AppAccessError"},
    {AppRequestError, "AppRequestError"},
    {PluginInfo, "PluginInfo"},
    {PluginRequest, "PluginRequest"}
};

std::string getCommandName(int command) {
    auto name = sCommandNames[command];
    return name;
}

EZY_NAMESPACE_END_WITH
