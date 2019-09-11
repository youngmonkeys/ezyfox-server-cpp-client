#ifndef EzyRequestSerializer_h
#define EzyRequestSerializer_h

#include "EzyRequest.h"
#include "../entity/EzyArray.h"

EZY_NAMESPACE_START_WITH(request)

class EzyRequest;

class EzyRequestSerializer {
public:
    virtual entity::EzyArray* serialize(int cmd, entity::EzyArray* data);
};

EZY_NAMESPACE_END_WITH

#endif /* EzyRequestSerializer_h */
