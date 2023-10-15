#include <thread>
#include <chrono>
#include "EzyClient.h"
#include "entity/EzyZone.h"
#include "entity/EzyUser.h"
#include "entity/EzyApp.h"
#include "entity/EzyPlugin.h"
#include "entity/EzyArray.h"
#include "setup/EzySetup.h"
#include "logger/EzyLogger.h"
#include "socket/EzyTcpSocketClient.h"
#include "socket/EzyPingSchedule.h"
#include "request/EzyRequestSerializer.h"
#include "manager/EzyHandlerManager.h"
#include "manager/EzyPingManager.h"
#include "manager/EzyAppManager.h"
#include "manager/EzyPluginManager.h"
#include "config/EzyClientConfig.h"
#include "constant/EzyDisconnectReason.h"

EZY_NAMESPACE_START

EzyClient::EzyClient() {
    mZone = 0;
    mMe = 0;
    mStatus = constant::Null;
    mUdpStatus = constant::Null;
}

EzyClient* EzyClient::create(config::EzyClientConfig *config) {
    auto pRet = new EzyClient();
    pRet->init(config);
    return pRet;
}

void EzyClient::init(config::EzyClientConfig *config) {
    mConfig = config;
    mName = config->getClientName();
    mPingManager = new manager::EzyPingManager(config->getPing());
    mPingSchedule = new socket::EzyPingSchedule(this);
    mHandlerManager = new manager::EzyHandlerManager(this);
    mRequestSerializer = new request::EzyRequestSerializer();
    mSetup = new setup::EzySetup(mHandlerManager);
    mUnloggableCommands.insert(constant::Ping);
    mUnloggableCommands.insert(constant::Pong);
    mSocketClient = newSocketClient();
}

EzyClient::~EzyClient() {
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

void EzyClient::destroy() {
    if(!isClientDestroyable(mStatus)) {
        logger::log("client is not in a destroyable status");
        return;
    }
    delete this;
}

void EzyClient::connect(std::string host, int port) {
    if(!isClientConnectable(mStatus)) {
        logger::log("client has already connected to: %s:%d", host.c_str(), port);
        return;
    }
    preconnect();
    setStatus(constant::Connecting);
    mSocketClient->connectTo(host, port);
}

bool EzyClient::reconnect() {
    if(!isClientReconnectable(mStatus)) {
        auto host = mSocketClient->getHost();
        auto port = mSocketClient->getPort();
        logger::log("client has already connected to: %s:%d", host.c_str(), port);
        return false;
    }
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
    auto socketClient = newTcpSocketClient();
    socketClient->setPingSchedule(mPingSchedule);
    socketClient->setPingManager(mPingManager);
    socketClient->setHandlerManager(mHandlerManager);
    socketClient->setConfig(mConfig->getSocket());
    socketClient->setReconnectConfig(mConfig->getReconnect());
    socketClient->setUnloggableCommands(mUnloggableCommands);
    return socketClient;
}

socket::EzySocketClient* EzyClient::newTcpSocketClient() {
    return new socket::EzyTcpSocketClient();
}

void EzyClient::close() {
    disconnect();
}

void EzyClient::disconnect(int reason) {
    mSocketClient->disconnect(reason);
}

void EzyClient::processEvents() {
    mSocketClient->processEventMessages();
}

void EzyClient::send(request::EzyRequest *request, bool encrypted) {
    auto cmd = request->getCommand();
    auto data = request->serialize();
    send(cmd, data, encrypted);
}

void EzyClient::send(constant::EzyCommand cmd, entity::EzyArray* data, bool encrypted) {
    auto array = mRequestSerializer->serialize(cmd, data);
    if(mSocketClient) {
        mSocketClient->sendMessage(array, encrypted);
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

setup::EzySetup* EzyClient::setup() {
    return mSetup;
}

entity::EzyApp* EzyClient::getApp() {
    if(mZone) {
        auto appManager = mZone->getAppManager();
        auto app = appManager->getApp();
        return app;
    }
    return 0;
}

entity::EzyApp* EzyClient::getAppById(int appId) {
    if(mZone) {
        auto appManager = mZone->getAppManager();
        auto app = appManager->getAppById(appId);
        return app;
    }
    return 0;
}

entity::EzyPlugin* EzyClient::getPluginById(int pluginId) {
    if(mZone) {
        auto pluginManager = mZone->getPluginManager();
        auto plugin = pluginManager->getPluginById(pluginId);
        return plugin;
    }
    return 0;
}

socket::EzySocketClient* EzyClient::getSocket() {
    return mSocketClient;
}

void EzyClient::setSessionId(int64_t sessionId) {
    mSessionId = sessionId;
    mSocketClient->setSessionId(mSessionId);
}

void EzyClient::setSessionToken(std::string token) {
    mSessionToken = token;
    mSocketClient->setSessionToken(mSessionToken);
}

void EzyClient::setSessionKey(std::string sessionKey) {
    mSessionKey = sessionKey;
    mSocketClient->setSessionKey(sessionKey);
}

bool EzyClient::isConnected() {
    return mStatus == constant::Connected;
}

bool EzyClient::isUdpConnected() {
    return mUdpStatus == constant::Connected;
}

void EzyClient::udpConnect(int port) {
    logger::log("only support TCP, use EzyUTClient instead");
}

void EzyClient::udpConnect(std::string host, int port) {
    logger::log("only support TCP, use EzyUTClient instead");
}

void EzyClient::udpSend(request::EzyRequest* request, bool encrypted) {
    logger::log("only support TCP, use EzyUTClient instead");
}

void EzyClient::udpSend(constant::EzyCommand cmd, entity::EzyArray* data, bool encrypted) {
    logger::log("only support TCP, use EzyUTClient instead");
}

EZY_NAMESPACE_END
