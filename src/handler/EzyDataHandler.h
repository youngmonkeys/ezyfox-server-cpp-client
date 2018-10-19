#pragma once

#include "../EzyMacro.h"
#include "../entity/EzyValue.h"

EZY_NAMESPACE_START_WITH(handler)

class EzyDataHandler {
public:
    virtual ~EzyDataHandler();
    virtual void handle(entity::EzyArray* data) = 0;
};

EZY_NAMESPACE_END_WITH
