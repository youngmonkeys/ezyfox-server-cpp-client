#include "EzyHandlerManager.h"
#include "../EzyClient.h"
#include "../socket/EzyPingSchedule.h"
#include "../handler/EzyDataHandler.h"
#include "../handler/EzyDataHandlers.h"
#include "../handler/EzyEventHandler.h"
#include "../handler/EzyEventHandlers.h"
#include "../handler/EzyAppDataHandlers.h"
#include "../handler/EzyPluginDataHandlers.h"

EZY_NAMESPACE_START_WITH(manager)

EzyHandlerManager::EzyHandlerManager(EzyClient* client) {
    this->mClient = client;
    this->mAppDataHandlerss.clear();
    this->mPluginDataHandlerss.clear();
    this->mEventHandlers = newEventHandlers();
    this->mDataHandlers = newDataHandlers();
}

EzyHandlerManager::~EzyHandlerManager() {
    EZY_SAFE_DELETE(mEventHandlers);
    EZY_SAFE_DELETE(mDataHandlers);
    EZY_FOREACH_MAP(mAppDataHandlerss)
        EZY_SAFE_DELETE(it->second);
    EZY_FOREACH_MAP(mPluginDataHandlerss)
        EZY_SAFE_DELETE(it->second);
    mClient = 0;
    mAppDataHandlerss.clear();
    mPluginDataHandlerss.clear();
}

handler::EzyEventHandlers* EzyHandlerManager::newEventHandlers() {
    auto handlers = new handler::EzyEventHandlers(mClient);
    handlers->addHandler(event::ConnectionSuccess, new handler::EzyConnectionSuccessHandler());
    handlers->addHandler(event::ConnectionFailure, new handler::EzyConnectionFailureHandler());
    handlers->addHandler(event::Disconnection, new handler::EzyDisconnectionHandler());
    return handlers;
}

handler::EzyDataHandlers* EzyHandlerManager::newDataHandlers() {
    auto handlers = new handler::EzyDataHandlers(mClient);
    handlers->addHandler(constant::Pong, new handler::EzyPongHandler());
    handlers->addHandler(constant::Login, new handler::EzyLoginSuccessHandler());
    handlers->addHandler(constant::AppAccess, new handler::EzyAppAccessHandler());
    handlers->addHandler(constant::AppExit, new handler::EzyAppExitHandler());
    handlers->addHandler(constant::AppRequest, new handler::EzyAppResponseHandler());
    handlers->addHandler(constant::PluginInfo, new handler::EzyPluginInfoHandler());
    handlers->addHandler(constant::PluginRequest, new handler::EzyPluginResponseHandler());
    return handlers;
}

handler::EzyDataHandler* EzyHandlerManager::getDataHandler(constant::EzyCommand cmd) {
    auto handler = mDataHandlers->getHandler(cmd);
    return handler;
}

handler::EzyEventHandler* EzyHandlerManager::getEventHandler(event::EzyEventType eventType) {
    auto handler = mEventHandlers->getHandler(eventType);
    return handler;
}

handler::EzyAppDataHandlers* EzyHandlerManager::getAppDataHandlers(std::string appName) {
    auto handlers = mAppDataHandlerss[appName];
    if(!handlers) {
        handlers = new handler::EzyAppDataHandlers();
        mAppDataHandlerss[appName] = handlers;
    }
    return handlers;
}

handler::EzyPluginDataHandlers* EzyHandlerManager::getPluginDataHandlers(std::string pluginName) {
    auto handlers = mPluginDataHandlerss[pluginName];
    if(!handlers){
        handlers = new handler::EzyPluginDataHandlers();
        mPluginDataHandlerss[pluginName] = handlers;
    }
    return handlers;
}

void EzyHandlerManager::addDataHandler(constant::EzyCommand cmd, handler::EzyDataHandler* dataHandler) {
    mDataHandlers->addHandler(cmd, dataHandler);
}

void EzyHandlerManager::addEventHandler(event::EzyEventType eventType, handler::EzyEventHandler* eventHandler) {
    mEventHandlers->addHandler(eventType, eventHandler);
}

EZY_NAMESPACE_END_WITH
