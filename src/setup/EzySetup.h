#pragma once

#include <string>
#include <map>
#include "../EzyMacro.h"
#include "../constant/EzyCommand.h"
#include "../event/EzyEventType.h"

EZY_NAMESPACE_START_WITH_ONLY(handler)
class EzyDataHandler;
class EzyEventHandler;
class EzyAppDataHandler;
class EzyAppDataHandlers;
class EzyPluginDataHandler;
class EzyPluginDataHandlers;
EZY_NAMESPACE_END_WITH

EZY_NAMESPACE_START_WITH_ONLY(manager)
class EzyHandlerManager;
EZY_NAMESPACE_END_WITH

EZY_NAMESPACE_START_WITH(setup)

class EzyAppSetup;
class EzyPluginSetup;

class EzySetup {
protected:
    manager::EzyHandlerManager* mHandlerManager;
    std::map<std::string, EzyAppSetup*> mAppSetups;
    std::map<std::string, EzyPluginSetup*> mPluginSetups;
public:
    EzySetup(manager::EzyHandlerManager* handlerManager);
    ~EzySetup();
    EzySetup* addDataHandler(constant::EzyCommand cmd, handler::EzyDataHandler* dataHandler);
    EzySetup* addEventHandler(event::EzyEventType eventType, handler::EzyEventHandler* eventHandler);
    EzyAppSetup* setupApp(std::string appName);
    EzyPluginSetup* setupPlugin(std::string pluginName);
};

//============================

class EzyAppSetup {
protected:
    EzySetup* mParent;
    handler::EzyAppDataHandlers* mDataHandlers;
public:
    EzyAppSetup(handler::EzyAppDataHandlers* dataHandlers, EzySetup* parent);
    ~EzyAppSetup();
    EzyAppSetup* addDataHandler(std::string cmd, handler::EzyAppDataHandler* dataHandler);
    EzySetup* done();
};

//============================

class EzyPluginSetup {
protected:
    EzySetup* mParent;
    handler::EzyPluginDataHandlers* mDataHandlers;
public:
    EzyPluginSetup(handler::EzyPluginDataHandlers* dataHandlers, EzySetup* parent);
    ~EzyPluginSetup();
    EzyPluginSetup* addDataHandler(std::string cmd, handler::EzyPluginDataHandler* dataHandler);
    EzySetup* done();
};


EZY_NAMESPACE_END_WITH
