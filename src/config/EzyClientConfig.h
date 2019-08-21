#pragma once

#include <string>
#include "../EzyMacro.h"

EZY_NAMESPACE_START_WITH_ONLY(config)

class EzySocketConfig;
class EzyReconnectConfig;

class EzyClientConfig {
protected:
    EZY_SYNTHESIZE(std::string, ZoneName);
    EZY_SYNTHESIZE_WRITEONLY(std::string, ClientName);
    EZY_SYNTHESIZE_READONLY(EzySocketConfig*, Socket);
    EZY_SYNTHESIZE_READONLY(EzyReconnectConfig*, Reconnect);
public:
    static EzyClientConfig* create();
    EzyClientConfig();
    ~EzyClientConfig();
    std::string getClientName();
    void setSocket(EzySocketConfig* socket);
    void setReconnect(EzyReconnectConfig* reconnect);
};

//============================

class EzySocketConfig {
protected:
    EZY_SYNTHESIZE(int, ReadBufferSize);
    EZY_SYNTHESIZE(int, ReadReserveSize);
    EZY_SYNTHESIZE(int, EncodeReserveSize);
    EZY_SYNTHESIZE(int, DecodeReserveSize);
public:
    EzySocketConfig();
};

//============================

class EzyReconnectConfig {
protected:
    EZY_SYNTHESIZE_BOOL(Enable);
    EZY_SYNTHESIZE(int, MaxReconnectCount);
    EZY_SYNTHESIZE(int, ReconnectPeriod);
public:
    EzyReconnectConfig();
};

EZY_NAMESPACE_END_WITH
