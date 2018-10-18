#pragma once

#include "EzyDataDeserializable.h"
#include "../entity/EzyArray.h"

EZY_NAMESPACE_START_WITH(io)

class EzyArrayDataDeserializable : public EzyDataDeserializable<entity::EzyArray> {
public:
    virtual void deserialize(entity::EzyArray* array) = 0;
};

EZY_NAMESPACE_END_WITH
