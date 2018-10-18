#pragma once

#include <string>
#include "../EzyMacro.h"
#include "EzyValue.h"

EZY_NAMESPACE_START
class EzyClient;
EZY_NAMESPACE_END

EZY_NAMESPACE_START_WITH(entity)

class EzyZone;

class EzyApp {
protected:
    EZY_SYNTHESIZE(int, Id);
    EZY_SYNTHESIZE(std::string, Name);
    EZY_SYNTHESIZE_READONLY(EzyZone*, Zone);
    EZY_SYNTHESIZE_READONLY(EzyClient*, Client);
public:
    EzyApp(EzyZone* zone);
    ~EzyApp();
    void send(entity::EzyValue* data);
    void send(int cmd, entity::EzyValue* data);
    void send(std::string cmd, entity::EzyValue* data);
};

EZY_NAMESPACE_END_WITH
