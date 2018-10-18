#pragma once

#include "../entity/EzyArray.h"
#include "EzyRequestSerializer.h"

EZY_NAMESPACE_START_WITH(request)
    
class EzyArrayRequestSerializer : public EzyRequestSerializer<entity::EzyArray> {
public:
    virtual entity::EzyArray* serialize(EzyRequest* request);
};

EZY_NAMESPACE_END_WITH
