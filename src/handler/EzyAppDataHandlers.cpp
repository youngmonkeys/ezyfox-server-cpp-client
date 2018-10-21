#include "EzyAppDataHandlers.h"
#include "EzyAppDataHandler.h"
#include "../logger/EzyLogger.h"

EZY_NAMESPACE_START_WITH(handler)

EzyAppDataHandlers::~EzyAppDataHandlers() {
        EZY_FOREACH_MAP(mHandlersByInt) {
            EZY_SAFE_DELETE(it->second);
        }
        EZY_FOREACH_MAP(mHandlersByString) {
            EZY_SAFE_DELETE(it->second);
        }
        mHandlersByInt.clear();
        mHandlersByString.clear();
}

void EzyAppDataHandlers::handleByInt(entity::EzyApp* app, entity::EzyArray* data) {
    auto cmd = (int)data->getInt(0);
    auto responseData = data->getItem(1, 0);
    auto handler = mHandlersByInt[cmd];
    if(handler)
        handler->handle(app, responseData);
    else
        logger::log("has no handler for command: %d", cmd);

}

void EzyAppDataHandlers::handleByString(entity::EzyApp* app, entity::EzyArray* data) {
    auto cmd = data->getString(0);
    auto responseData = data->getItem(1, 0);
    auto handler = mHandlersByString[cmd];
    if(handler)
        handler->handle(app, responseData);
    else
        logger::log("has no handler for command: %s", cmd.c_str());

}

void EzyAppDataHandlers::addHandler(int cmd, EzyAppDataHandler* handler) {
    auto old = mHandlersByInt[cmd];
    mHandlersByInt[cmd] = handler;
    if(old) EZY_SAFE_DELETE(old);
}

void EzyAppDataHandlers::addHandler(std::string cmd, EzyAppDataHandler* handler) {
    auto old = mHandlersByString[cmd];
    mHandlersByString[cmd] = handler;
    if(old) EZY_SAFE_DELETE(old);
}

EZY_NAMESPACE_END_WITH
