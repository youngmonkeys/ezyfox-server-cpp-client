#pragma once

#include "../EzyMacro.h"

EZY_NAMESPACE_START_WITH(io)

template <class T>
class EzyDataSerializable {
public:
    virtual T* serialize() = 0;
};

EZY_NAMESPACE_END_WITH
