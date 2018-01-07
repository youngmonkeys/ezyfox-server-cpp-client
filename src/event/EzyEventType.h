#pragma once

#include "../EzyMacro.h"

EZY_NAMESPACE_START
namespace event {
    
enum EzyEventType {
    connectionSuccess,
    connectionFailure,
    handshake
};
    
}
EZY_NAMESPACE_END
