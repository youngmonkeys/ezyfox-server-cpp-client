#include "../logger/EzyLogger.h"
#include "EzyAppDataHandlers.h"

EZY_NAMESPACE_START_WITH(handler)
    
EzyAppDataHandlers::EzyAppDataHandlers() {
}

EzyAppDataHandlers::~EzyAppDataHandlers() {
    EZY_FOREACH_MAP(handlersByInt) {
        EZY_SAFE_DELETE(it->second);
    }
    EZY_FOREACH_MAP(handlersByString) {
        EZY_SAFE_DELETE(it->second);
    }
    handlersByInt.clear();
    handlersByString.clear();
}
    
void EzyAppDataHandlers::handleByInt(entity::EzyArray* data) {
    auto cmd = (int)data->getInt(0);
    auto responseData = data->getArray(1);
    auto handler = handlersByInt[cmd];
    if(handler)
        handler->handle(responseData);
    else
        logger::log("has no handler for command: %d", cmd);
    
}

void EzyAppDataHandlers::handleByString(entity::EzyArray* data) {
    auto cmd = data->getString(0);
    auto responseData = data->getArray(1);
    auto handler = handlersByString[cmd];
    if(handler)
        handler->handle(responseData);
    else
        logger::log("has no handler for command: %s", cmd.c_str());
    
}

void EzyAppDataHandlers::addHandler(int cmd, EzyAppDataHandler *handler) {
    handlersByInt[cmd] = handler;
}

void EzyAppDataHandlers::addHandler(std::string cmd, EzyAppDataHandler *handler) {
    handlersByString[cmd] = handler;
}

EZY_NAMESPACE_END_WITH
