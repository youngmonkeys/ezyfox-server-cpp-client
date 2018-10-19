#include "EzyDataHandler.h"
#include "../EzyClient.h"
#include "../request/EzyRequest.h"
#include "../socket/EzyPingSchedule.h"

EZY_NAMESPACE_START_WITH(handler)

EzyDataHandler::~EzyDataHandler() {
}

void EzyDataHandler::setClient(EzyClient *client) {
    this->mClient = client;
}

//===============================================
void EzyHandshakeHandler::handle(entity::EzyArray* data) {
    mPingSchedule->start();
    handleLogin(data);
    postHandle(data);
}

void EzyHandshakeHandler::handleLogin(entity::EzyArray* data) {
    auto request = getLoginRequest();
    mClient->send(request);
}

void EzyHandshakeHandler::setClient(EzyClient* client) {
    EzyDataHandler::setClient(client);
    mPingSchedule = client->getPingSchedule();
}

void EzyHandshakeHandler::postHandle(entity::EzyArray* data) {
}
//===============================================

EZY_NAMESPACE_END_WITH
