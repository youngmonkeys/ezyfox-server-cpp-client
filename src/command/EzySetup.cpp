#include "EzySetup.h"
#include "../manager/EzyHandlerManager.h"
#include "../handler/EzyDataHandler.h"
#include "../handler/EzyEventHandler.h"
#include "../handler/EzyAppDataHandler.h"
#include "../handler/EzyAppDataHandlers.h"

EZY_NAMESPACE_START_WITH(command)

EzySetup::EzySetup(manager::EzyHandlerManager* handlerManager) {
    this->mHandlerManager = handlerManager;
}

EzySetup::~EzySetup() {
    this->mHandlerManager = nullptr;
    EZY_FOREACH_MAP(mAppSetups) {
        EZY_SAFE_DELETE(it->second);
    }
    mAppSetups.clear();
}

EzySetup* EzySetup::addDataHandler(constant::EzyCommand cmd, handler::EzyDataHandler* dataHandler) {
    this->mHandlerManager->addDataHandler(cmd, dataHandler);
    return this;
}

EzySetup* EzySetup::addEventHandler(event::EzyEventType eventType, handler::EzyEventHandler* eventHandler) {
    this->mHandlerManager->addEventHandler(eventType, eventHandler);
    return this;
}

EzyAppSetup* EzySetup::setupApp(std::string appName) {
    auto appSetup = mAppSetups[appName];
    if(!appSetup) {
        auto appDataHandlers = mHandlerManager->getAppDataHandlers(appName);
        appSetup = new EzyAppSetup(appDataHandlers, this);
        mAppSetups[appName] = appSetup;
    }
    return appSetup;
}

//============================

EzyAppSetup::EzyAppSetup(handler::EzyAppDataHandlers* dataHandlers, EzySetup* parent) {
    this->mDataHandlers = dataHandlers;
    this->mParent = parent;
}

EzyAppSetup::~EzyAppSetup() {
    this->mParent = nullptr;
    this->mDataHandlers = nullptr;
}

EzyAppSetup* EzyAppSetup::addDataHandler(int cmd, handler::EzyAppDataHandler* dataHandler) {
    this->mDataHandlers->addHandler(cmd, dataHandler);
    return this;
}

EzyAppSetup* EzyAppSetup::addDataHandler(std::string cmd, handler::EzyAppDataHandler* dataHandler) {
    this->mDataHandlers->addHandler(cmd, dataHandler);
    return this;
}

EzySetup* EzyAppSetup::done() {
    return this->mParent;
}

EZY_NAMESPACE_END_WITH
