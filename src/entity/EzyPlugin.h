#pragma once

#include <string>
#include "EzyMacro.h"
#include "EzyValue.h"

EZY_NAMESPACE_START
class EzyClient;
EZY_NAMESPACE_END

EZY_NAMESPACE_START_WITH(entity)

class EzyZone;

class EzyPlugin {
protected:
    EZY_SYNTHESIZE(int, Id);
    EZY_SYNTHESIZE(std::string, Name);
    EZY_SYNTHESIZE_READONLY(EzyZone*, Zone);
    EZY_SYNTHESIZE_READONLY(EzyClient*, Client);
public:
    EzyPlugin(EzyClient* client);
    ~EzyPlugin();
    void sendById(entity::EzyValue* data);
    void sendById(int cmd, entity::EzyValue* data);
    void sendById(std::string cmd, entity::EzyValue* data);
    void sendByName(entity::EzyValue* data);
    void sendByName(int cmd, entity::EzyValue* data);
    void sendByName(std::string cmd, entity::EzyValue* data);
};

EZY_NAMESPACE_END_WITH

