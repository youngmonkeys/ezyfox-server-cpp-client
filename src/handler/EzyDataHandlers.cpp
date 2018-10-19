#include "../logger/EzyLogger.h"
#include "EzyDataHandlers.h"

EZY_NAMESPACE_START_WITH(handler)
    
EzyDataHandlers::EzyDataHandlers() {
}

EzyDataHandlers::~EzyDataHandlers() {
    for(auto it = handlers.begin() ; it != handlers.end() ; ++it) {
        EZY_SAFE_DELETE(it->second);
    }
    handlers.clear();
}
    
void EzyDataHandlers::handle(entity::EzyArray *data) {
    auto cmdId = data->getInt(0);
    auto cmd = (constant::EzyCommand)cmdId;
    auto responseData = data->getArray(1);
    auto handler = handlers[cmd];
    if(handler)
        handler->handle(responseData);
    else
        logger::log("has no handler for command: %d", cmd);
    
}

void EzyDataHandlers::addHandler(constant::EzyCommand cmd, EzyDataHandler *handler) {
    handlers[cmd] = handler;
}

EZY_NAMESPACE_END_WITH
