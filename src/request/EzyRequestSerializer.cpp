#include "EzyRequestSerializer.h"

EZY_NAMESPACE_START_WITH(request)

entity::EzyArray* EzyRequestSerializer::serialize(int cmd, entity::EzyArray* data) {
    auto array = new entity::EzyArray();
    array->addInt(cmd);
    array->addItem(data);
    return array;
}

EZY_NAMESPACE_END_WITH

