//
//  EzySocketDelegate.h
//  ezyfox-server-cpp-client
//
//  Created by Dung Ta Van on 8/10/19.
//  Copyright © 2019 Young Monkeys. All rights reserved.
//

#ifndef EzySocketDelegate_h
#define EzySocketDelegate_h

#include "../EzyMacro.h"

EZY_NAMESPACE_START_WITH(socket)

class EzySocketDelegate {
public:
    virtual void onDisconnected(int reason = 0) = 0;
};

EZY_NAMESPACE_END_WITH

#endif /* EzySocketDelegate_h */
