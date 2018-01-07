#pragma once

#include "../EzyMacro.h"
#include "EzyRequest.h"

EZY_NAMESPACE_START
namespace request {
 
class EzyRequestDeliver {
public:
    virtual void send(EzyRequest *request) = 0;
};
    
}
EZY_NAMESPACE_END
