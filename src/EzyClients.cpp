#include "EzyClients.h"
#include "EzyClient.h"
#include "config/EzyClientConfig.h"

EZY_NAMESPACE_START

EzyClients::EzyClients() {
    mClients.clear();
    mDefaultClientName = "";
}

EzyClient* EzyClients::newClient(config::EzyClientConfig* config) {
    std::unique_lock<std::mutex> lock(mMutex);
    return newClient0(config);
}

EzyClient* EzyClients::newClient0(config::EzyClientConfig* config) {
    auto clientName = config->getClientName();
    auto client = mClients[clientName];
    if(!client) {
        client = EzyClient::create(config);
        addClient0(client);
        if(mDefaultClientName.length() == 0)
            mDefaultClientName = client->getName();
    }
    return client;
}

EzyClient* EzyClients::newDefaultClient(config::EzyClientConfig* config) {
    std::unique_lock<std::mutex> lock(mMutex);
    auto client = newClient0(config);
    mDefaultClientName = client->getName();
    return client;
}

void EzyClients::addClient(EzyClient* client) {
    std::unique_lock<std::mutex> lock(mMutex);
    addClient0(client);
}

void EzyClients::addClient0(EzyClient* client) {
    mClients[client->getName()] = client;
}

void EzyClients::removeClient(EzyClient *client) {
    std::unique_lock<std::mutex> lock(mMutex);
    mClients.erase(client->getName());
}

void EzyClients::destroyClient(EzyClient *client) {
    removeClient(client);
    client->destroy();
}

EzyClient* EzyClients::getClient(std::string name) {
    std::unique_lock<std::mutex> lock(mMutex);
    return getClient0(name);
}

EzyClient* EzyClients::getClient0(std::string name) {
    auto answer = mClients[name];
    return answer;
}

EzyClient* EzyClients::getDefaultClient() {
    std::unique_lock<std::mutex> lock(mMutex);
    if(mDefaultClientName.length() == 0)
        return NULL;
    auto client = getClient0(mDefaultClientName);
    return client;
}

void EzyClients::getClients(std::vector<EzyClient*>& cachedClients) {
    cachedClients.clear();
    std::unique_lock<std::mutex> lock(mMutex);
    EZY_FOREACH_MAP(mClients) {
        auto client = it->second;
        if(client)
            cachedClients.push_back(it->second);
    }
}

EZY_NAMESPACE_END
