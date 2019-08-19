#pragma once

#include <string>
#include "../EzyMacro.h"
#include "EzyValue.h"

EZY_NAMESPACE_START
class EzyClient;
EZY_NAMESPACE_END

EZY_NAMESPACE_START_WITH_ONLY(handler)
class EzyPluginDataHandlers;
EZY_NAMESPACE_END_WITH

EZY_NAMESPACE_START_WITH(entity)

class EzyZone;

class EzyPlugin {
protected:
    EZY_SYNTHESIZE(int, Id);
    EZY_SYNTHESIZE(std::string, Name);
    EZY_SYNTHESIZE_READONLY(EzyZone*, Zone);
    EZY_SYNTHESIZE_READONLY(EzyClient*, Client);
    EZY_SYNTHESIZE_READONLY(handler::EzyPluginDataHandlers*, DataHandlers);
public:
    EzyPlugin(EzyZone* zone, int identifier, std::string name);
    ~EzyPlugin();
    void send(entity::EzyValue* data);
    void send(std::string cmd, entity::EzyValue* data);
};

EZY_NAMESPACE_END_WITH

