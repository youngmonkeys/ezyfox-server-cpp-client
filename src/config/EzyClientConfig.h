#pragma once

#include <string>
#include "../EzyMacro.h"

EZY_NAMESPACE_START_WITH_ONLY(config)

class EzyReconnectConfig;

class EzyClientConfig {
protected:
    EZY_SYNTHESIZE(std::string, ZoneName);
    EZY_SYNTHESIZE_WRITEONLY(std::string, ClientName);
    EZY_SYNTHESIZE_READONLY(EzyReconnectConfig*, Reconnect);
public:
    static EzyClientConfig* create();
    EzyClientConfig();
    ~EzyClientConfig();
    std::string getClientName();
    void setReconnect(EzyReconnectConfig* reconnect);
};

class EzyReconnectConfig {
protected:
    EZY_SYNTHESIZE_BOOL(Enable);
    EZY_SYNTHESIZE(int, MaxReconnectCount);
    EZY_SYNTHESIZE(int, ReconnectPeriod);
public:
    EzyReconnectConfig();
};

EZY_NAMESPACE_END_WITH
