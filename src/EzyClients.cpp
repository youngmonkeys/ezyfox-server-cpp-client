#include "EzyClients.h"
#include "EzyClient.h"
#include "config/EzyClientConfig.h"

EZY_NAMESPACE_START

EzyClients::EzyClients() {
    mDefaultClientName = "";
}

EzyClient* EzyClients::newClient(config::EzyClientConfig* config) {
    auto client = new EzyClient(config);
    if(mDefaultClientName.length() == 0)
        mDefaultClientName = client->getName();
    addClient(client);
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

void EzyClients::getClients(std::vector<EzyClient*>& buffer) {
    buffer.clear();
    EZY_FOREACH_MAP(mClients)
        buffer.push_back(it->second);
}

EZY_NAMESPACE_END
