#pragma once

#include "../EzyMacro.h"

EZY_NAMESPACE_START_WITH_ONLY(io)

template <class T>
class EzyDataDeserializable {
public:
    virtual void deserialize(T* t) = 0;
};

EZY_NAMESPACE_END_WITH
