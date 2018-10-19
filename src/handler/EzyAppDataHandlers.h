#pragma once

#include <map>
#include "../EzyMacro.h"
#include "../event/EzyEvent.h"
#include "../event/EzyEventType.h"
#include "../constant/EzyCommand.h"
#include "EzyAppDataHandler.h"
#include "../entity/EzyArray.h"

EZY_NAMESPACE_START_WITH(handler)

class EzyAppDataHandler;

class EzyAppDataHandlers {
protected:
    std::map<int, EzyAppDataHandler*> handlersByInt;
    std::map<std::string, EzyAppDataHandler*> handlersByString;
public:
    EzyAppDataHandlers();
    ~EzyAppDataHandlers();
    void handleByInt(entity::EzyArray* data);
    void handleByString(entity::EzyArray* data);
    void addHandler(int cmd, EzyAppDataHandler* handler);
    void addHandler(std::string cmd, EzyAppDataHandler* handler);
};

EZY_NAMESPACE_END_WITH
