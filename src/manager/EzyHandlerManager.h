#pragma once

#include <string>
#include <map>
#include "../EzyMacro.h"
#include "../constant/EzyCommand.h"
#include "../event/EzyEventType.h"

EZY_NAMESPACE_START
class EzyClient;
EZY_NAMESPACE_END

EZY_NAMESPACE_START_WITH_ONLY(socket)
class EzyPingSchedule;
EZY_NAMESPACE_END_WITH

EZY_NAMESPACE_START_WITH_ONLY(handler)
class EzyDataHandler;
class EzyEventHandler;
class EzyDataHandlers;
class EzyEventHandlers;
class EzyAppDataHandlers;
EZY_NAMESPACE_END_WITH

EZY_NAMESPACE_START_WITH(manager)

class EzyHandlerManager {
protected:
    EzyClient* mClient;
    std::map<std::string, handler::EzyAppDataHandlers*> mAppDataHandlerss;
protected:
    EZY_SYNTHESIZE_READONLY(handler::EzyDataHandlers*, DataHandlers);
    EZY_SYNTHESIZE_READONLY(handler::EzyEventHandlers*, EventHandlers);
protected:
    handler::EzyEventHandlers* newEventHandlers();
    handler::EzyDataHandlers* newDataHandlers();
public:
    EzyHandlerManager(EzyClient* client);
    handler::EzyDataHandler* getDataHandler(constant::EzyCommand cmd);
    handler::EzyEventHandler* getEventHandler(event::EzyEventType eventType);
    handler::EzyAppDataHandlers* getAppDataHandlers(std::string appName);
    void addDataHandler(constant::EzyCommand cmd, handler::EzyDataHandler* dataHandler);
    void addEventHandler(event::EzyEventType eventType, handler::EzyEventHandler* eventHandler);
};

EZY_NAMESPACE_END_WITH
