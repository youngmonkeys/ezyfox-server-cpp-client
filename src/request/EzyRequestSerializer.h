#pragma once

#include "EzyRequest.h"
#include "../io/EzyArrayDataSerializable.h"

EZY_NAMESPACE_START
namespace request {
 
template <class T>
class EzyRequestSerializer {
public:
    virtual T* serialize(EzyRequest* request) = 0;
};
    
}
EZY_NAMESPACE_END
