#include "EzyEventHandler.h"
#include "../constant/EzyConnectionStatus.h"
#include "../request/EzyRequest.h"
#include "../EzyClient.h"

EZY_NAMESPACE_START_WITH(handler)

EzyEventHandler::~EzyEventHandler() {
}

void EzyEventHandler::setClient(EzyClient* client) {
    this->mClient = client;
}

//==========================================================
void EzyConnectionSuccessHandler::process(event::EzyEvent* evt)
{
    updateConnectionStatus();
    sendHandshakeRequest();
    postHandle();
}

void EzyConnectionSuccessHandler::updateConnectionStatus()
{
    mClient->setStatus(constant::EzyConnectionStatus::Connected);
}

void EzyConnectionSuccessHandler::postHandle()
{
}

void EzyConnectionSuccessHandler::sendHandshakeRequest()
{
    auto request = newHandshakeRequest();
    mClient->send(request);
}

request::EzyRequest* EzyConnectionSuccessHandler::newHandshakeRequest(){
    auto request = request::EzyHandshakeRequest::create();
    request->setClientId(getClientId());
    request->setClientKey(getClientKey());
    request->setClientType("CPP");
    request->setClientVersion("1.0.0");
    request->setEnableEncryption(isEnableEncryption());
    request->setToken(getStoredToken());
    return request;
}

std::string EzyConnectionSuccessHandler::getClientId() {
    return "unset";
}

std::string EzyConnectionSuccessHandler::getClientKey() {
    return "";
}

bool EzyConnectionSuccessHandler::isEnableEncryption() {
    return false;
}

std::string EzyConnectionSuccessHandler::getStoredToken() {
    return "";
}
//==========================================================

EZY_NAMESPACE_END_WITH