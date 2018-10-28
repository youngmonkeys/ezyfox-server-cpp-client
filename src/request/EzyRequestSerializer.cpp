#include "EzyRequestSerializer.h"

EZY_NAMESPACE_START_WITH(request)

entity::EzyArray* EzyRequestSerializer::serialize(int cmd, entity::EzyArray* data) {
    auto array = entity::EzyArray::create();
    array->addInt(cmd);
    array->addItem(data);
    array->retain();
    return array;
}

EZY_NAMESPACE_END_WITH

