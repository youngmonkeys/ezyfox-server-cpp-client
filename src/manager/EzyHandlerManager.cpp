#include "EzyHandlerManager.h"
#include "../EzyClient.h"
#include "../socket/EzyPingSchedule.h"
#include "../handler/EzyDataHandler.h"
#include "../handler/EzyDataHandlers.h"
#include "../handler/EzyEventHandler.h"
#include "../handler/EzyEventHandlers.h"
#include "../handler/EzyAppDataHandler.h"
#include "../handler/EzyAppDataHandlers.h"

EZY_NAMESPACE_START_WITH(manager)

EzyHandlerManager::EzyHandlerManager(EzyClient* client) {
    this->mClient = client;
    this->mEventHandlers = new handler::EzyEventHandlers(client);
    this->mDataHandlers = new handler::EzyDataHandlers();
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

void EzyHandlerManager::addDataHandler(constant::EzyCommand cmd, handler::EzyDataHandler* dataHandler) {
    mDataHandlers->addHandler(cmd, dataHandler);
}

void EzyHandlerManager::addEventHandler(event::EzyEventType eventType, handler::EzyEventHandler* eventHandler) {
    mEventHandlers->addHandler(eventType, eventHandler);
}

EZY_NAMESPACE_END_WITH
