//
//  EzyThread.cpp
//  ezyfox-server-cpp-client
//
//  Created by Dung Ta Van on 8/8/19.
//  Copyright © 2019 Young Monkeys. All rights reserved.
//

#ifdef PF_APPLE
#import <Foundation/NSString.h>
#import <Foundation/NSThread.h>
#endif

#include "EzyThread.h"

EZY_NAMESPACE_START_WITH_ONLY(concurrent)

void EzyThread::setCurrentThreadName(std::string name) {
#ifdef PF_APPLE
    [[NSThread currentThread] setName: [NSString stringWithUTF8String: name.c_str()]];
#endif
}

EZY_NAMESPACE_END_WITH
