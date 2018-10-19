#pragma once

#include "EzyRequest.h"
#include "../entity/EzyArray.h"

EZY_NAMESPACE_START_WITH(request)

class EzyRequest;

class EzyRequestSerializer {
public:
    virtual entity::EzyArray* serialize(EzyRequest* request);
};

EZY_NAMESPACE_END_WITH
