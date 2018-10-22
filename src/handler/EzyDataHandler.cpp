#include "EzyDataHandler.h"
#include "../EzyClient.h"
#include "../request/EzyRequest.h"
#include "../socket/EzyPingSchedule.h"
#include "../entity/EzyZone.h"
#include "../entity/EzyApp.h"
#include "../entity/EzyUser.h"
#include "../manager/EzyHandlerManager.h"
#include "../manager/EzyAppManager.h"
#include "../handler/EzyAppDataHandlers.h"

EZY_NAMESPACE_START_WITH(handler)

EzyDataHandler::~EzyDataHandler() {
    this->mClient = 0;
}

void EzyDataHandler::setClient(EzyClient *client) {
    this->mClient = client;
}

//===============================================

void EzyPongHandler::handle(entity::EzyArray *data) {
}

//===============================================
void EzyHandshakeHandler::handle(entity::EzyArray* data) {
    mPingSchedule->start();
    handleLogin(data);
    postHandle(data);
}

void EzyHandshakeHandler::handleLogin(entity::EzyArray* data) {
    auto request = getLoginRequest();
    mClient->send(request);
}

void EzyHandshakeHandler::setClient(EzyClient* client) {
    EzyDataHandler::setClient(client);
    mPingSchedule = client->getPingSchedule();
}

void EzyHandshakeHandler::postHandle(entity::EzyArray* data) {
}
//===============================================

//===============================================

void EzyLoginSuccessHandler::handle(entity::EzyArray *data) {
    auto joinedApps = data->getArray(4);
    auto responseData = data->getItem(5);
    auto user = newUser(data);
    auto zone = newZone(data);
    mClient->setMe(user);
    mClient->setZone(zone);
    handleResponseAppDatas(joinedApps);
    handleResponseData(responseData);
    if (joinedApps->size() == 0)
        handleLoginSuccess(responseData);
    else
        handleReconnectSuccess(responseData);
}

void EzyLoginSuccessHandler::handleResponseData(entity::EzyValue* responseData) {
}

void EzyLoginSuccessHandler::handleResponseAppDatas(entity::EzyArray* appDatas) {
    auto handlerManager = mClient->getHandlerManager();
    auto appAccessHandler = handlerManager->getDataHandler(constant::AppAccess);
    for (int i = 0; i < appDatas->size(); i++)
    {
        auto appData = appDatas->getArray(i);
        auto accessAppData = newAccessAppData(appData);
        appAccessHandler->handle(accessAppData);
    }
}

entity::EzyArray* EzyLoginSuccessHandler::newAccessAppData(entity::EzyArray* appData) {
    return appData;
}

entity::EzyUser* EzyLoginSuccessHandler::newUser(entity::EzyArray* data) {
    auto userId = data->getInt(2);
    auto username = data->getString(3);
    auto user = new entity::EzyUser(userId, username);
    return user;
}

entity::EzyZone* EzyLoginSuccessHandler::newZone(entity::EzyArray* data) {
    auto zoneId = data->getInt(0);
    auto zoneName = data->getString(1);
    auto zone = new entity::EzyZone(mClient, (int)zoneId, zoneName);
    return zone;
}

void EzyLoginSuccessHandler::handleLoginSuccess(entity::EzyValue* responseData) {
}

void EzyLoginSuccessHandler::handleReconnectSuccess(entity::EzyValue* responseData) {
    handleLoginSuccess(responseData);
}

//===============================================

void EzyAccessAppHandler::handle(entity::EzyArray* data) {
    auto zone = mClient->getZone();
    auto appManager = zone->getAppManager();
    auto app = newApp(zone, data);
    appManager->addApp(app);
    mClient->addApp(app);
    postHandle(app, data);
}

void EzyAccessAppHandler::postHandle(entity::EzyApp* app, entity::EzyArray* data) {
}

entity::EzyApp* EzyAccessAppHandler::newApp(entity::EzyZone* zone, entity::EzyArray* data) {
    auto appId = data->getInt(0);
    auto appName = data->getString(1);
    auto app = new entity::EzyApp(zone, (int)appId, appName);
    return app;
}

//===============================================

void EzyAbstractAppResponseHandler::handle(entity::EzyArray* data) {
    auto appId = data->getInt(0);
    auto responseData = data->getArray(1);
    auto app = mClient->getAppById((int)appId);
    auto dataHandlers = app->getDataHandlers();
    handle(dataHandlers, app, responseData);
}

void EzyAppResponseByIntHandler::handle(EzyAppDataHandlers* handlers,
                                        entity::EzyApp* app,
                                        entity::EzyArray* data) {
    handlers->handleByInt(app, data);
}

void EzyAppResponseByStringHandler::handle(EzyAppDataHandlers* handlers,
                                           entity::EzyApp* app,
                                           entity::EzyArray* data) {
    handlers->handleByString(app, data);
}

//===============================================

EZY_NAMESPACE_END_WITH
