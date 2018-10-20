#pragma once

#include <string>
#include "../EzyMacro.h"

EZY_NAMESPACE_START_WITH_ONLY(entity)

class EzyUser {
protected:
    EZY_SYNTHESIZE_READONLY(long, Id);
    EZY_SYNTHESIZE_READONLY(std::string, Name);
public:
    EzyUser(long identifier, std::string name);
};

EZY_NAMESPACE_END_WITH
