#ifndef EzyClients_h
#define EzyClients_h

#include <string>
#include <map>
#include <vector>
#include <mutex>
#include "EzyMacro.h"

EZY_NAMESPACE_START_WITH_ONLY(config)
class EzyClientConfig;
EZY_NAMESPACE_END_WITH

EZY_NAMESPACE_START

class EzyClient;

class EzyClients {
private:
    std::mutex mMutex;
    std::string mDefaultClientName;
    std::map<std::string, EzyClient*> mClients;
public:
    EZY_SINGLETON_GET_INSTANCE(EzyClients)
public:
    void addClient(EzyClient* client);
    void removeClient(EzyClient* client);
    void destroyClient(EzyClient* client);
    EzyClient* newClient(config::EzyClientConfig* config);
    EzyClient* newDefaultClient(config::EzyClientConfig* config);
    EzyClient* getClient(std::string name);
    EzyClient* getDefaultClient();
    void getClients(std::vector<EzyClient*>& cachedClients);
protected:
    void addClient0(EzyClient* client);
    EzyClient* newClient0(config::EzyClientConfig* config);
    EzyClient* getClient0(std::string name);
};
EZY_NAMESPACE_END

#endif /* EzyClients_h */
