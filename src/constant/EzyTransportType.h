//
//  EzyTransportType.h
//  ezyfox-server-cpp-client
//
//  Created by Dung Ta Van on 3/26/20.
//  Copyright © 2020 Young Monkeys. All rights reserved.
//

#ifndef EzyTransportType_h
#define EzyTransportType_h


#include "../EzyMacro.h"


EZY_NAMESPACE_START_WITH_ONLY(constant)

enum EzyTransportType {
    Tcp = 1,
    Udp = 2
};

EZY_NAMESPACE_END_WITH

#endif /* EzyTransportType_h */
