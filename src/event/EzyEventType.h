#pragma once

#include "../EzyMacro.h"

EZY_NAMESPACE_START
namespace event {
    
enum EzyEventType {
    ConnectionSuccess = 0,
    ConnectionFailure,
    Handshake,
    LoginSuccess,
    AccessAppSuccess,
};
    
}
EZY_NAMESPACE_END
