#pragma once

#include <string>
#include <map>
#include <vector>
#include "EzyMacro.h"

EZY_NAMESPACE_START_WITH_ONLY(config)
class EzyClientConfig;
EZY_NAMESPACE_END_WITH

EZY_NAMESPACE_START

class EzyClient;

class EzyClients {
private:
    std::string mDefaultClientName;
    std::map<std::string, EzyClient*> mClients;
public:
    EZY_SINGLETON_GET_INSTANCE(EzyClients)
public:
    void addClient(EzyClient* client);
    EzyClient* newClient(config::EzyClientConfig* config);
    EzyClient* newDefaultClient(config::EzyClientConfig* config);
    EzyClient* getClient(std::string name);
    EzyClient* getDefaultClient();
    std::vector<EzyClient*> getClients();
};
EZY_NAMESPACE_END

