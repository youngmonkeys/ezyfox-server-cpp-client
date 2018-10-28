#include "EzyPlugin.h"
#include "EzyZone.h"
#include "../EzyClient.h"
#include "../util/EzyMaps.h"
#include "../entity/EzyArray.h"
#include "../request/EzyRequest.h"

EZY_NAMESPACE_START_WITH(entity)

EzyPlugin::EzyPlugin(EzyClient* client) {
    this->mClient = client;
    this->mZone = client->getZone();
}

EzyPlugin::~EzyPlugin() {
}

void EzyPlugin::sendById(entity::EzyValue *data) {
    auto request = new request::EzyPluginRequestByIdRequest();
    request->setPluginId(mId);
    request->setData(data);
    mClient->send(request);
}

void EzyPlugin::sendById(int cmd, entity::EzyValue *data) {
    auto array = new entity::EzyArray();
    array->addInt(cmd);
    array->addItem(data);
    sendById(array);
}

void EzyPlugin::sendById(std::string cmd, entity::EzyValue *data) {
    auto array = new entity::EzyArray();
    array->addString(cmd);
    array->addItem(data);
    sendById(array);
}

void EzyPlugin::sendByName(entity::EzyValue *data) {
    auto request = new request::EzyPluginRequestByNameRequest();
    request->setPluginName(mName);
    request->setData(data);
    mClient->send(request);
}

void EzyPlugin::sendByName(int cmd, entity::EzyValue *data) {
    auto array = new entity::EzyArray();
    array->addInt(cmd);
    array->addItem(data);
    sendByName(array);
}

void EzyPlugin::sendByName(std::string cmd, entity::EzyValue *data) {
    auto array = new entity::EzyArray();
    array->addString(cmd);
    array->addItem(data);
    sendByName(array);
}

EZY_NAMESPACE_END_WITH
