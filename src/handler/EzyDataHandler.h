#pragma once

#include "../EzyMacro.h"

EZY_NAMESPACE_START_WITH_ONLY(handler)

template <class T>
class EzyDataHandler {
    
    void handle(T* data);
    
};

EZY_NAMESPACE_END_WITH
