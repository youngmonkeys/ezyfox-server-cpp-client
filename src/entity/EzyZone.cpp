#include "EzyZone.h"
#include "../EzyClient.h"
#include "../manager/EzyAppManager.h"
#include "../manager/EzyPluginManager.h"

EZY_NAMESPACE_START_WITH(entity)

EzyZone::EzyZone(EzyClient* client, int identifier, std::string name) {
    this->mId = identifier;
    this->mName = name;
    this->mClient = client;
    this->mAppManager = new manager::EzyAppManager();
    this->mPluginManager = new manager::EzyPluginManager();
}

EzyZone::~EzyZone() {
    EZY_SAFE_DELETE(mAppManager);
    EZY_SAFE_DELETE(mPluginManager);
}

EZY_NAMESPACE_END_WITH
