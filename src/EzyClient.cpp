#include <thread>
#include <chrono>
#include "EzyClient.h"
#include "entity/EzyApp.h"
#include "command/EzySetup.h"
#include "logger/EzyLogger.h"
#include "socket/EzyTcpSocketClient.h"
#include "socket/EzyPingSchedule.h"
#include "request/EzyRequestSerializer.h"
#include "manager/EzyHandlerManager.h"
#include "manager/EzyPingManager.h"

EZY_NAMESPACE_START

EzyClient::EzyClient() {
    mPingManager = new manager::EzyPingManager();
    mPingSchedule = new socket::EzyPingSchedule(this);
    mHandlerManager = new manager::EzyHandlerManager(this);
    mRequestSerializer = new request::EzyRequestSerializer();
    mSetup = new command::EzySetup(mHandlerManager);
}

EzyClient::~EzyClient() {
}

void EzyClient::connect(std::string host, int port) {
    mSocketClient = newSocketClient();
    mSocketClient->setHandlerManager(mHandlerManager);
    mSocketClient->connectTo(host, port);
}

socket::EzySocketClient* EzyClient::newSocketClient() {
    auto socketClient = new socket::EzyTcpSocketClient();
    socketClient->setHandlerManager(mHandlerManager);
    return socketClient;
}

void EzyClient::disconnect() {
    if(mSocketClient)
        mSocketClient->closeSocket();
    EZY_SAFE_DELETE(mSocketClient);
}

void EzyClient::processEvents() {
    if(mSocketClient)
        mSocketClient->processMessage();
}

void EzyClient::send(request::EzyRequest *request) {
    auto data = mRequestSerializer->serialize(request);
    if(mSocketClient)
        mSocketClient->sendMessage(data);
}

command::EzySetup* EzyClient::setup() {
    return mSetup;
}

void EzyClient::addApp(entity::EzyApp *app) {
    mAppsById[app->getId()] = app;
}

entity::EzyApp* EzyClient::getAppById(int appId) {
    auto app = mAppsById[appId];
    return app;
}

EZY_NAMESPACE_END
