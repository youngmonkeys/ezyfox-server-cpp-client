#pragma once

#include "EzyRequest.h"

EZY_NAMESPACE_START_WITH_ONLY(request)
 
template <class T>
class EzyRequestSerializer {
public:
    virtual T* serialize(EzyRequest* request) = 0;
};

EZY_NAMESPACE_END_WITH
