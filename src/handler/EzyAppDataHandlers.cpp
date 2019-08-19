#include "EzyAppDataHandlers.h"
#include "EzyAppDataHandler.h"
#include "../logger/EzyLogger.h"

EZY_NAMESPACE_START_WITH(handler)

EzyAppDataHandlers::EzyAppDataHandlers() {
    mHandlers.clear();
}

EzyAppDataHandlers::~EzyAppDataHandlers() {
    EZY_FOREACH_MAP(mHandlers)
        EZY_SAFE_DELETE(it->second);
    mHandlers.clear();
}

void EzyAppDataHandlers::handle(entity::EzyApp* app, entity::EzyArray* data) {
    auto cmd = data->getString(0);
    auto responseData = data->getItem(1, 0);
    auto handler = mHandlers[cmd];
    if(handler)
        handler->handle(app, responseData);
    else
        logger::log("has no handler for command: %s", cmd.c_str());

}

void EzyAppDataHandlers::addHandler(std::string cmd, EzyAppDataHandler* handler) {
    auto old = mHandlers[cmd];
    mHandlers[cmd] = handler;
    if(old) EZY_SAFE_DELETE(old);
}

EZY_NAMESPACE_END_WITH
