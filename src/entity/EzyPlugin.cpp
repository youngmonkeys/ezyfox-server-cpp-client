#include "EzyPlugin.h"
#include "EzyArray.h"
#include "EzyObject.h"
#include "EzyZone.h"
#include "../EzyClient.h"
#include "../util/EzyMaps.h"
#include "../request/EzyRequest.h"
#include "../handler/EzyPluginDataHandlers.h"
#include "../manager/EzyHandlerManager.h"

EZY_NAMESPACE_START_WITH(entity)

EzyPlugin::EzyPlugin(EzyZone* zone, int identifier, std::string name) {
    this->mId = identifier;
    this->mName = name;
    this->mZone = zone;
    this->mClient = zone->getClient();
    this->mDataHandlers = mClient->getHandlerManager()->getPluginDataHandlers(name);
}

EzyPlugin::~EzyPlugin() {
    this->mZone = 0;
    this->mClient = 0;
    this->mDataHandlers = 0;
}

void EzyPlugin::send(entity::EzyValue *data) {
    auto request = request::EzyPluginRequestRequest::create();
    request->setPluginId(mId);
    request->setData(data);
    mClient->send(request);
}

void EzyPlugin::send(std::string cmd) {
    send(cmd, new entity::EzyObject());
}

void EzyPlugin::send(std::string cmd, entity::EzyValue *data) {
    auto array = new entity::EzyArray();
    array->addString(cmd);
    array->addItem(data);
    send(array);
}

EZY_NAMESPACE_END_WITH
