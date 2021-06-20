#include "EzyApp.h"
#include "EzyArray.h"
#include "EzyObject.h"
#include "EzyZone.h"
#include "../request/EzyRequest.h"
#include "../util/EzyMaps.h"
#include "../EzyClient.h"
#include "../handler/EzyAppDataHandlers.h"
#include "../manager/EzyHandlerManager.h"

EZY_NAMESPACE_START_WITH(entity)

EzyApp::EzyApp(EzyZone* zone, int identifier, std::string name) {
    this->mId = identifier;
    this->mName = name;
    this->mZone = zone;
    this->mClient = zone->getClient();
    this->mDataHandlers = mClient->getHandlerManager()->getAppDataHandlers(name);
}

EzyApp::~EzyApp() {
    this->mZone = 0;
    this->mClient = 0;
    this->mDataHandlers = 0;
}

void EzyApp::send(entity::EzyValue *data, bool encrypted) {
    auto request = request::EzyAppRequestRequest::create();
    request->setAppId(mId);
    request->setData(data);
    mClient->send(request, encrypted);
}

void EzyApp::send(std::string cmd, bool encrypted) {
    send(cmd, new entity::EzyObject(), encrypted);
}

void EzyApp::send(std::string cmd, entity::EzyValue *data, bool encrypted) {
    auto array = new entity::EzyArray();
    array->addString(cmd);
    array->addItem(data);
    send(array, encrypted);
}

void EzyApp::udpSend(std::string cmd) {
    udpSend(cmd, new entity::EzyObject());
}

void EzyApp::udpSend(std::string cmd, entity::EzyValue* data) {
    auto commandData = new entity::EzyArray();
    commandData->addString(cmd);
    commandData->addItem(data);
    auto requestData = new entity::EzyArray();
    requestData->addInt(mId);
    requestData->addArray(commandData);
    mClient->udpSend(constant::AppRequest, requestData);
}

EZY_NAMESPACE_END_WITH
