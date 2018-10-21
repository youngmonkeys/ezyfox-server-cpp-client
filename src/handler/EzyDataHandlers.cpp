#include "../logger/EzyLogger.h"
#include "../EzyClient.h"
#include "EzyDataHandler.h"
#include "EzyDataHandlers.h"

EZY_NAMESPACE_START_WITH(handler)
    
EzyDataHandlers::EzyDataHandlers(EzyClient* client) {
    this->mClient = client;
}

EzyDataHandlers::~EzyDataHandlers() {
    EZY_FOREACH_MAP(mHandlers) {
        EZY_SAFE_DELETE(it->second);
    }
    mHandlers.clear();
}
    
void EzyDataHandlers::handle(constant::EzyCommand cmd, entity::EzyArray *data) {
    auto handler = mHandlers[cmd];
    if(handler)
        handler->handle(data);
    else
        logger::log("has no handler for command: %d", cmd);
}

EzyDataHandler* EzyDataHandlers::getHandler(constant::EzyCommand cmd) {
    auto handler = mHandlers[cmd];
    return handler;
}

void EzyDataHandlers::addHandler(constant::EzyCommand cmd, EzyDataHandler* handler) {
    handler->setClient(mClient);
    auto old = mHandlers[cmd];
    mHandlers[cmd] = handler;
    if(old) EZY_SAFE_DELETE(old);
}

EZY_NAMESPACE_END_WITH
