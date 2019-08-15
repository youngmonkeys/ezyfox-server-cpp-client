//
//  EzyThread.hpp
//  ezyfox-server-cpp-client
//
//  Created by Dung Ta Van on 8/8/19.
//  Copyright © 2019 Young Monkeys. All rights reserved.
//

#pragma once

#include <string>
#include "../EzyMacro.h"

EZY_NAMESPACE_START_WITH_ONLY(concurrent)

class EzyThread {
public:
    static void setCurrentThreadName(std::string name);
};

EZY_NAMESPACE_END_WITH
