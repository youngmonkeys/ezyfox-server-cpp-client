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
EZY_NAMESPACE_END_WITH

EZY_NAMESPACE_START_WITH_ONLY(manager)
class EzyHandlerManager;
EZY_NAMESPACE_END_WITH

EZY_NAMESPACE_START_WITH(command)

class EzyAppSetup;

class EzySetup {
protected:
    manager::EzyHandlerManager* mHandlerManager;
    std::map<std::string, EzyAppSetup*> mAppSetups;
public:
    EzySetup(manager::EzyHandlerManager* handlerManager);
    ~EzySetup();
    EzySetup* addDataHandler(constant::EzyCommand cmd, handler::EzyDataHandler* dataHandler);
    EzySetup* addEventHandler(event::EzyEventType eventType, handler::EzyEventHandler* eventHandler);
    EzyAppSetup* setupApp(std::string appName);
};

class EzyAppSetup {
protected:
    EzySetup* mParent;
    handler::EzyAppDataHandlers* mDataHandlers;
public:
    EzyAppSetup(handler::EzyAppDataHandlers* dataHandlers, EzySetup* parent);
    ~EzyAppSetup();
    EzyAppSetup* addDataHandler(int cmd, handler::EzyAppDataHandler* dataHandler);
    EzyAppSetup* addDataHandler(std::string cmd, handler::EzyAppDataHandler* dataHandler);
    EzySetup* done();
};

EZY_NAMESPACE_END_WITH
