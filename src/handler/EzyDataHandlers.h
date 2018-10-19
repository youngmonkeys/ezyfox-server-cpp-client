#pragma once

#include <map>
#include "../EzyMacro.h"
#include "../event/EzyEvent.h"
#include "../event/EzyEventType.h"
#include "../constant/EzyCommand.h"
#include "../entity/EzyArray.h"

EZY_NAMESPACE_START
class EzyClient;
EZY_NAMESPACE_END

EZY_NAMESPACE_START_WITH(handler)

class EzyDataHandler;

class EzyDataHandlers {
protected:
    EzyClient* mClient;
    std::map<constant::EzyCommand, EzyDataHandler*> mHandlers;
public:
    EzyDataHandlers(EzyClient* client);
    ~EzyDataHandlers();
    void handle(entity::EzyArray* data);
    EzyDataHandler* getHandler(constant::EzyCommand cmd);
    void addHandler(constant::EzyCommand cmd, EzyDataHandler* handler);
};

EZY_NAMESPACE_END_WITH
