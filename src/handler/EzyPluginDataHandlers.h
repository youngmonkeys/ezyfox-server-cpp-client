//
//  EzyPluginDataHandlers.h
//  ezyfox-server-cpp-client
//
//  Created by Dung Ta Van on 8/19/19.
//  Copyright © 2019 Young Monkeys. All rights reserved.
//

#ifndef EzyPluginDataHandlers_h
#define EzyPluginDataHandlers_h

#include <map>
#include "../EzyMacro.h"
#include "../event/EzyEvent.h"
#include "../event/EzyEventType.h"
#include "../constant/EzyCommand.h"
#include "../entity/EzyArray.h"

EZY_NAMESPACE_START_WITH_ONLY(entity)
class EzyPlugin;
EZY_NAMESPACE_END_WITH

EZY_NAMESPACE_START_WITH_ONLY(handler)

class EzyPluginDataHandler;

class EzyPluginDataHandlers {
protected:
    std::map<std::string, EzyPluginDataHandler*> mHandlers;
public:
    EzyPluginDataHandlers();
    ~EzyPluginDataHandlers();
    void handle(entity::EzyPlugin* plugin, entity::EzyArray* data);
    void addHandler(std::string cmd, EzyPluginDataHandler* handler);
};

EZY_NAMESPACE_END_WITH

#endif /* EzyPluginDataHandlers_h */
