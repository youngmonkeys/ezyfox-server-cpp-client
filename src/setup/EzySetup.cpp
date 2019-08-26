#include "EzySetup.h"
#include "../manager/EzyHandlerManager.h"
#include "../handler/EzyDataHandler.h"
#include "../handler/EzyEventHandler.h"
#include "../handler/EzyAppDataHandler.h"
#include "../handler/EzyAppDataHandlers.h"
#include "../handler/EzyPluginDataHandler.h"
#include "../handler/EzyPluginDataHandlers.h"

EZY_NAMESPACE_START_WITH(setup)

EzySetup::EzySetup(manager::EzyHandlerManager* handlerManager) {
    this->mAppSetups.clear();
    this->mPluginSetups.clear();
    this->mHandlerManager = handlerManager;
}

EzySetup::~EzySetup() {
    EZY_FOREACH_MAP(mAppSetups)
        EZY_SAFE_DELETE(it->second);
    EZY_FOREACH_MAP(mPluginSetups)
        EZY_SAFE_DELETE(it->second);
    this->mAppSetups.clear();
    this->mPluginSetups.clear();
    this->mHandlerManager = 0;
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

EzyPluginSetup* EzySetup::setupPlugin(std::string pluginName) {
    auto pluginSetup = mPluginSetups[pluginName];
    if(!pluginSetup) {
        auto pluginDataHandlers = mHandlerManager->getPluginDataHandlers(pluginName);
        pluginSetup = new EzyPluginSetup(pluginDataHandlers, this);
        mPluginSetups[pluginName] = pluginSetup;
    }
    return pluginSetup;
}

//============================

EzyAppSetup::EzyAppSetup(handler::EzyAppDataHandlers* dataHandlers, EzySetup* parent) {
    this->mDataHandlers = dataHandlers;
    this->mParent = parent;
}

EzyAppSetup::~EzyAppSetup() {
    this->mParent = 0;
    this->mDataHandlers = 0;
}

EzyAppSetup* EzyAppSetup::addDataHandler(std::string cmd, handler::EzyAppDataHandler* dataHandler) {
    this->mDataHandlers->addHandler(cmd, dataHandler);
    return this;
}

EzySetup* EzyAppSetup::done() {
    return this->mParent;
}

//============================

EzyPluginSetup::EzyPluginSetup(handler::EzyPluginDataHandlers* dataHandlers, EzySetup* parent) {
    this->mDataHandlers = dataHandlers;
    this->mParent = parent;
}

EzyPluginSetup::~EzyPluginSetup() {
    this->mParent = 0;
    this->mDataHandlers = 0;
}

EzyPluginSetup* EzyPluginSetup::addDataHandler(std::string cmd, handler::EzyPluginDataHandler* dataHandler) {
    this->mDataHandlers->addHandler(cmd, dataHandler);
    return this;
}

EzySetup* EzyPluginSetup::done() {
    return this->mParent;
}

EZY_NAMESPACE_END_WITH
