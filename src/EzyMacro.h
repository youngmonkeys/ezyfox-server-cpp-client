//
//  EzyMacro.h
//  ezyfox-server-cpp-client
//
//  Created by Dung Ta Van on 11/28/17.
//  Copyright © 2017 Young Monkeys. All rights reserved.
//

#pragma once

#define EZY_NAMESPACE com::tvd12::ezyfoxserver::client

#define EZY_NAMESPACE_START \
namespace com {\
namespace tvd12 {\
namespace ezyfoxserver {\
namespace client {\

#define EZY_NAMESPACE_END \
}\
}\
}\
}

#define EZY_DELETE_0(variable) \
if (variable) {\
delete variable;\
variable = 0;\
}
