#pragma once

#include <map>
#include "../EzyMacro.h"
#include "../event/EzyEvent.h"
#include "../event/EzyEventType.h"
#include "../constant/EzyCommand.h"
#include "../entity/EzyArray.h"

EZY_NAMESPACE_START_WITH_ONLY(entity)
class EzyApp;
EZY_NAMESPACE_END_WITH

EZY_NAMESPACE_START_WITH_ONLY(handler)

class EzyAppDataHandler;

class EzyAppDataHandlers {
protected:
        std::map<std::string, EzyAppDataHandler*> mHandlers;
public:
    EzyAppDataHandlers();
    ~EzyAppDataHandlers();
    void handle(entity::EzyApp* app, entity::EzyArray* data);
    void addHandler(std::string cmd, EzyAppDataHandler* handler);
};

EZY_NAMESPACE_END_WITH
