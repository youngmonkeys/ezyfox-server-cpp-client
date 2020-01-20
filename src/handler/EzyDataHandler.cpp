#include "EzyDataHandler.h"
#include "../logger/EzyLogger.h"
#include "../EzyClient.h"
#include "../request/EzyRequest.h"
#include "../socket/EzyPingSchedule.h"
#include "../entity/EzyZone.h"
#include "../entity/EzyApp.h"
#include "../entity/EzyPlugin.h"
#include "../entity/EzyUser.h"
#include "../manager/EzyHandlerManager.h"
#include "../manager/EzyAppManager.h"
#include "../manager/EzyPluginManager.h"
#include "../handler/EzyAppDataHandlers.h"
#include "../handler/EzyPluginDataHandlers.h"

EZY_NAMESPACE_START_WITH(handler)

EzyDataHandler::EzyDataHandler() {
    this->mClient = 0;
}

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
EzyHandshakeHandler::EzyHandshakeHandler() {
    this->mPingSchedule = 0;
}

EzyHandshakeHandler::~EzyHandshakeHandler() {
    this->mPingSchedule = 0;
}

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
    auto responseData = data->getItem(4);
    auto user = newUser(data);
    auto zone = newZone(data);
    mClient->setMe(user);
    mClient->setZone(zone);
    handleLoginSuccess(responseData);
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

//===============================================

void EzyLoginErrorHandler::handle(entity::EzyArray *data) {
    mClient->disconnect(401);
    handleLoginError(data);
}

void EzyLoginErrorHandler::handleLoginError(entity::EzyArray *data) {
}

//===============================================

void EzyAppAccessHandler::handle(entity::EzyArray* data) {
    auto zone = mClient->getZone();
    auto appManager = zone->getAppManager();
    auto app = newApp(zone, data);
    appManager->addApp(app);
    postHandle(app, data);
    logger::log("access app: %s successfully", app->getName().c_str());
}

void EzyAppAccessHandler::postHandle(entity::EzyApp* app, entity::EzyArray* data) {
}

entity::EzyApp* EzyAppAccessHandler::newApp(entity::EzyZone* zone, entity::EzyArray* data) {
    auto appId = data->getInt(0);
    auto appName = data->getString(1);
    auto app = new entity::EzyApp(zone, (int)appId, appName);
    return app;
}

//===============================================

void EzyAppExitHandler::handle(entity::EzyArray* data) {
    auto zone = mClient->getZone();
    auto appManager = zone->getAppManager();
    auto appId = data->getInt(0);
    auto reasonId = data->getInt(1);
    auto app = appManager->removeApp((int)appId);
    if(app) {
        postHandle(app, data);
        logger::log("user exit app: %s, reason: %d", app->getName().c_str(), reasonId);
    }
}

void EzyAppExitHandler::postHandle(entity::EzyApp* app, entity::EzyArray* data) {
}

//===============================================

void EzyPluginInfoHandler::handle(entity::EzyArray* data) {
    auto zone = mClient->getZone();
    auto pluginManager = zone->getPluginManager();
    auto plugin = newPlugin(zone, data);
    pluginManager->addPlugin(plugin);
    postHandle(plugin, data);
    logger::log("request plugin: %s successfully", plugin->getName().c_str());
}

void EzyPluginInfoHandler::postHandle(entity::EzyPlugin* app, entity::EzyArray* data) {
}

entity::EzyPlugin* EzyPluginInfoHandler::newPlugin(entity::EzyZone* zone, entity::EzyArray* data) {
    auto pluginId = data->getInt(0);
    auto pluginName = data->getString(1);
    auto plugin = new entity::EzyPlugin(zone, (int)pluginId, pluginName);
    return plugin;
}

//===============================================

void EzyAppResponseHandler::handle(entity::EzyArray* data) {
    auto appId = data->getInt(0);
    auto responseData = data->getArray(1);
    auto app = mClient->getAppById((int)appId);
    if(!app) {
        logger::log("receive message when has not joined app yet");
        return;
    }
    auto dataHandlers = app->getDataHandlers();
    dataHandlers->handle(app, responseData);
}

//===============================================

void EzyPluginResponseHandler::handle(entity::EzyArray* data) {
    auto pluginId = data->getInt(0);
    auto responseData = data->getArray(1);
    auto plugin = mClient->getPluginById((int)pluginId);
    auto dataHandlers = plugin->getDataHandlers();
    dataHandlers->handle(plugin, responseData);
}


EZY_NAMESPACE_END_WITH
