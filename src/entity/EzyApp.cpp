#include "EzyApp.h"
#include "EzyArray.h"
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
}

void EzyApp::send(entity::EzyValue *data) {
    auto request = new request::EzyAppRequestRequest();
    request->setAppId(mId);
    request->setData(data);
    mClient->send(request);
}

void EzyApp::send(int cmd, entity::EzyValue *data) {
    auto array = new entity::EzyArray();
    array->addInt(cmd);
    array->addItem(data);
    send(array);
}

void EzyApp::send(std::string cmd, entity::EzyValue *data) {
    auto array = new entity::EzyArray();
    array->addString(cmd);
    array->addItem(data);
    send(array);
}

EZY_NAMESPACE_END_WITH
