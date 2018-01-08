#pragma once

#include "EzyDataSerializable.h"
#include "../entity/EzyArray.h"

EZY_NAMESPACE_START
namespace io {
    
class EzyArrayDataSerializable : public EzyDataSerializable<entity::EzyArray> {
public:
    virtual entity::EzyArray* serialize() = 0;
};
    
}
EZY_NAMESPACE_END
