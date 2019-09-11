//
//  EzyMainEventsLoop.h
//  ezyfox-server-cpp-client
//
//  Created by Dung Ta Van on 8/4/19.
//  Copyright © 2019 Young Monkeys. All rights reserved.
//

#ifndef EzyMainEventsLoop_h
#define EzyMainEventsLoop_h

#include <vector>
#include "../EzyMacro.h"

EZY_NAMESPACE_START
class EzyClient;
class EzyClients;
EZY_NAMESPACE_END

EZY_NAMESPACE_START_WITH(socket)

class EzyMainEventsLoop {
protected:
    bool mActive;
    EzyClients* mClients;
    std::vector<EzyClient*> mCachedClients;
public:
    EzyMainEventsLoop();
    virtual ~EzyMainEventsLoop();
    
    void start(int sleepTime = 3);
    void stop();
protected:
    void processEvents(int sleepTime);
};

EZY_NAMESPACE_END_WITH

#endif /* EzyMainEventsLoop_h */
