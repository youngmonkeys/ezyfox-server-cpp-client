#include "../logger/EzyLogger.h"
#include "EzyDataHandlers.h"

EZY_NAMESPACE_START_WITH(handler)
    
EzyDataHandlers::EzyDataHandlers() {
}

EzyDataHandlers::~EzyDataHandlers() {
    EZY_FOREACH_MAP(mHandlers) {
        EZY_SAFE_DELETE(it->second);
    }
    mHandlers.clear();
}
    
void EzyDataHandlers::handle(entity::EzyArray *data) {
    auto cmdId = data->getInt(0);
    auto cmd = (constant::EzyCommand)cmdId;
    auto responseData = data->getArray(1);
    auto handler = mHandlers[cmd];
    if(handler)
        handler->handle(responseData);
    else
        logger::log("has no handler for command: %d", cmd);
}

EzyDataHandler* EzyDataHandlers::getHandler(constant::EzyCommand cmd) {
    auto handler = mHandlers[cmd];
    return handler;
}

void EzyDataHandlers::addHandler(constant::EzyCommand cmd, EzyDataHandler *handler) {
    mHandlers[cmd] = handler;
}

EZY_NAMESPACE_END_WITH
