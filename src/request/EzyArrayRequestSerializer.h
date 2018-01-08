#pragma once

#include "EzyRequestSerializer.h"
#include "../entity/EzyArray.h"

EZY_NAMESPACE_START
namespace request {
    
class EzyArrayRequestSerializer : public EzyRequestSerializer<entity::EzyArray> {
public:
    virtual entity::EzyArray* serialize(EzyRequest* request);
};
    
}
EZY_NAMESPACE_END
