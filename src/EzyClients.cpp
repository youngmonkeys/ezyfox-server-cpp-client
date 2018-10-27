#include "EzyClients.h"
#include "EzyClient.h"
#include "config/EzyClientConfig.h"

EZY_NAMESPACE_START

EzyClients::EzyClients() {
    mDefaultClientName = "";
}

EzyClient* EzyClients::newClient(config::EzyClientConfig* config) {
    auto client = new EzyClient(config);
    addClient(client);
    if(mDefaultClientName.length() == 0)
        mDefaultClientName = client->getName();
    return client;
}

EzyClient* EzyClients::newDefaultClient(config::EzyClientConfig* config) {
    auto client = newClient(config);
    mDefaultClientName = client->getName();
    return client;
}

void EzyClients::addClient(EzyClient* client) {
    mClients[client->getName()] = client;
}

EzyClient* EzyClients::getClient(std::string name) {
    auto answer = mClients[name];
    return answer;
}

EzyClient* EzyClients::getDefaultClient() {
    auto client = getClient(mDefaultClientName);
    return client;
}

std::vector<EzyClient*> EzyClients::getClients() {
    auto clients = std::vector<EzyClient*>();
    EZY_FOREACH_MAP(mClients)
        clients.push_back(it->second);
    return clients;
}

EZY_NAMESPACE_END
