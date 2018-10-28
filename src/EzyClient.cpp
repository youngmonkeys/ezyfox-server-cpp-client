#include <thread>
#include <chrono>
#include "EzyClient.h"
#include "entity/EzyZone.h"
#include "entity/EzyUser.h"
#include "entity/EzyApp.h"
#include "entity/EzyArray.h"
#include "command/EzySetup.h"
#include "logger/EzyLogger.h"
#include "socket/EzyTcpSocketClient.h"
#include "socket/EzyPingSchedule.h"
#include "request/EzyRequestSerializer.h"
#include "manager/EzyHandlerManager.h"
#include "manager/EzyPingManager.h"
#include "config/EzyClientConfig.h"
#include "constant/EzyDisconnectReason.h"

EZY_NAMESPACE_START

EzyClient::EzyClient(config::EzyClientConfig* config) {
    mZone = 0;
    mMe = 0;
    mConfig = config;
    mName = config->getClientName();
    mSocketClient = 0;
    mPingManager = new manager::EzyPingManager();
    mPingSchedule = new socket::EzyPingSchedule(this);
    mHandlerManager = new manager::EzyHandlerManager(this);
    mRequestSerializer = new request::EzyRequestSerializer();
    mSetup = new command::EzySetup(mHandlerManager);
    mUnloggableCommands.insert(constant::Ping);
    mUnloggableCommands.insert(constant::Pong);
}

EzyClient::~EzyClient() {
    mAppsById.clear();
    EZY_SAFE_DELETE(mConfig);
    EZY_SAFE_DELETE(mSetup);
    EZY_SAFE_DELETE(mRequestSerializer);
    EZY_SAFE_DELETE(mSocketClient);
    EZY_SAFE_DELETE(mHandlerManager);
    EZY_SAFE_DELETE(mPingManager);
    EZY_SAFE_DELETE(mPingSchedule);
    EZY_SAFE_DELETE(mZone);
    EZY_SAFE_DELETE(mMe);
}

void EzyClient::connect(std::string host, int port) {
    preconnect();
    mSocketClient = newSocketClient();
    mSocketClient->connectTo(host, port);
}

bool EzyClient::reconnect() {
    this->preconnect();
    auto success = mSocketClient->reconnect();
    if (success)
        setStatus(constant::Reconnecting);
    return success;
}

void EzyClient::preconnect() {
    EZY_SAFE_DELETE(mZone);
    EZY_SAFE_DELETE(mMe);
}

socket::EzySocketClient* EzyClient::newSocketClient() {
    auto socketClient = new socket::EzyTcpSocketClient();
    socketClient->setPingSchedule(mPingSchedule);
    socketClient->setPingManager(mPingManager);
    socketClient->setHandlerManager(mHandlerManager);
    socketClient->setReconnectConfig(mConfig->getReconnect());
    socketClient->setUnloggableCommands(mUnloggableCommands);
    return socketClient;
}

void EzyClient::disconnect() {
    if(mSocketClient)
        mSocketClient->onDisconnected(constant::UnknownDisconnection);
    EZY_SAFE_DELETE(mSocketClient);
}

void EzyClient::processEvents() {
    if(mSocketClient)
        mSocketClient->processEventMessages();
}

void EzyClient::send(request::EzyRequest *request) {
    auto cmd = request->getCommand();
    auto data = request->serialize();
    send(cmd, data);
}

void EzyClient::send(constant::EzyCommand cmd, entity::EzyArray* data) {
    auto array = mRequestSerializer->serialize(cmd, data);
    if(mSocketClient) {
        mSocketClient->sendMessage(array);
        printSentData(cmd, data);
    }
}

void EzyClient::printSentData(int cmd, entity::EzyArray *data) {
#ifdef EZY_DEBUG
    if(mUnloggableCommands.count(cmd) > 0)
        return;
    auto cmdName = constant::getCommandName(cmd);
    std::ostringstream stream;
    stream << "\n-------------------\n";
    stream << "[SEND] ==>\n";
    stream << "command: ";
    stream << cmdName;
    stream << ", data:\n";
    logger::console(stream.str().c_str());
    stream.str("");
    stream.clear();
    data->printDebug();
    logger::console("\n----------------\n");
#endif
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

void EzyClient::setStatus(constant::EzyConnectionStatus status) {
    std::unique_lock<std::mutex> lock(mStatusMutex);
    mStatus = status;
}

constant::EzyConnectionStatus EzyClient::getStatus() {
    std::unique_lock<std::mutex> lock(mStatusMutex);
    return mStatus;
}

EZY_NAMESPACE_END
