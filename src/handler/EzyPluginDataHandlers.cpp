//
//  EzyPluginDataHandlers.cpp
//  ezyfox-server-cpp-client
//
//  Created by Dung Ta Van on 8/19/19.
//  Copyright © 2019 Young Monkeys. All rights reserved.
//

#include "EzyPluginDataHandlers.h"
#include "EzyPluginDataHandler.h"
#include "../logger/EzyLogger.h"

EZY_NAMESPACE_START_WITH(handler)

EzyPluginDataHandlers::EzyPluginDataHandlers() {
    mHandlers.clear();
}

EzyPluginDataHandlers::~EzyPluginDataHandlers() {
    EZY_FOREACH_MAP(mHandlers)
        EZY_SAFE_DELETE(it->second);
    mHandlers.clear();
}

void EzyPluginDataHandlers::handle(entity::EzyPlugin* plugin, entity::EzyArray* data) {
    auto cmd = data->getString(0);
    auto responseData = data->getItem(1, 0);
    auto handler = mHandlers[cmd];
    if(handler)
        handler->handle(plugin, responseData);
    else
        logger::log("has no handler for command: %s", cmd.c_str());
    
}

void EzyPluginDataHandlers::addHandler(std::string cmd, EzyPluginDataHandler* handler) {
    auto old = mHandlers[cmd];
    mHandlers[cmd] = handler;
    if(old) EZY_SAFE_DELETE(old);
}

EZY_NAMESPACE_END_WITH

