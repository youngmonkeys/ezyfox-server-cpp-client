#include <iostream>
#include <thread>
#include <chrono>
#include "EzyClient.h"
#include "socket/EzySocketTcpClient.h"

using namespace EZY_NAMESPACE;

//==========================================================

class ConnectionSuccessEventHandler :
public handler::EzyEventHandler<event::EzyConnectionSuccessEvent> {
protected:
    request::EzyRequestDeliver *mRequestDeliver;
protected:
    virtual void sendHandShakeRequest();
public:
    ConnectionSuccessEventHandler(request::EzyRequestDeliver *requestDeliver);
    virtual void handle(event::EzyConnectionSuccessEvent *event);
};

//==========================================================

class HandshakeEventHandler :
public handler::EzyEventHandler<event::EzyHandshakeEvent> {
protected:
    request::EzyRequestDeliver *mRequestDeliver;
protected:
    virtual void sendLoginRequest();
public:
    HandshakeEventHandler(request::EzyRequestDeliver* requestDeliver);
    virtual void handle(event::EzyHandshakeEvent* event);
};

//==========================================================

class LoginEventHandler :
public handler::EzyEventHandler<event::EzyLoginEvent> {
protected:
    request::EzyRequestDeliver *mRequestDeliver;
protected:
    virtual void sendAccessAppRequest();
public:
    LoginEventHandler(request::EzyRequestDeliver* requestDeliver);
    virtual void handle(event::EzyLoginEvent* event);
};

//==========================================================

class AccessAppEventHandler :
public handler::EzyEventHandler<event::EzyLoginEvent> {
protected:
    EzyAppManager* mAppManager;
    request::EzyRequestDeliver *mRequestDeliver;
public:
    AccessAppEventHandler(EzyAppManager* appManager,
                          request::EzyRequestDeliver* requestDeliver);
    virtual void handle(event::EzyAccessAppEvent* event);
};

//==========================================================

void ConnectionSuccessEventHandler::handle(event::EzyConnectionSuccessEvent *event) {
    sendHandShakeRequest();
}

void ConnectionSuccessEventHandler::sendHandShakeRequest() {
    auto keyPairGentor = new socket::EzyRsaKeyPairGentor();
    auto keyPair = keyPairGentor->generate(512);
    auto clientKey = keyPair->getPublicKey();
    logger::log("public key: %s", clientKey.c_str());
    auto params = new request::EzyHandshakeRequestParams();
    params->setClientId("clientId");
    params->setClientKey(clientKey);
    params->setReconnectToken("reconectToken");
    auto request = request::EzyHandshakeRequest::create(params);
    mRequestDeliver->send(request);
}

ConnectionSuccessEventHandler::
ConnectionSuccessEventHandler(request::EzyRequestDeliver *requestDeliver) {
    this->mRequestDeliver = requestDeliver;
}

//==========================================================

HandshakeEventHandler::
HandshakeEventHandler(request::EzyRequestDeliver *requestDeliver) {
    this->mRequestDeliver = requestDeliver;
}

void HandshakeEventHandler::handle(event::EzyHandshakeEvent *event) {
    sendLoginRequest();
    handler::EzyPingSchedule *pingSchedule = new handler::EzyPingSchedule(mRequestDeliver);
    pingSchedule->setPeriod(5);
    pingSchedule->start();
}

void HandshakeEventHandler::sendLoginRequest() {
    auto params = new request::EzyLoginRequestParams();
    params->setUsername("dungtv");
    params->setPassword("123456");
    params->setData(new entity::EzyArray());
    auto request = request::EzyLoginRequest::create(params);
    mRequestDeliver->send(request);
}

//==========================================================

LoginEventHandler::LoginEventHandler(request::EzyRequestDeliver* requestDeliver) {
    this->mRequestDeliver = requestDeliver;
}

void LoginEventHandler::handle(event::EzyLoginEvent *event) {
    sendAccessAppRequest();
}

void LoginEventHandler::sendAccessAppRequest() {
    auto params = new request::EzyAccessAppRequestParams();
    params->setAppName("ezyfox-simple-chat");
    params->setData(new entity::EzyArray());
    auto request = request::EzyAccessAppRequest::create(params);
    mRequestDeliver->send(request);
}

//==========================================================

AccessAppEventHandler::AccessAppEventHandler(EzyAppManager* appManager,
                                             request::EzyRequestDeliver* requestDeliver) {
    this->mAppManager = appManager;
    this->mRequestDeliver = requestDeliver;
}

void AccessAppEventHandler::handle(event::EzyAccessAppEvent *event) {
    auto args = event->getArgs();
    auto app = new EzyApp();
    app->setId(args->getAppId());
    app->setName(args->getAppName());
    mAppManager->addApp(app);
}

//==========================================================

int main(int argc, const char * argv[]) {
    srand( static_cast<unsigned int>(time(NULL)));
    EzyClient *client = new EzyClient();
    client->addEventHandler(event::ConnectionSuccess, new ConnectionSuccessEventHandler(client));
    client->addEventHandler(event::Handshake, new HandshakeEventHandler(client));
    client->addEventHandler(event::LoginSuccess, new LoginEventHandler(client));
    logger::log("start client");
    client->connect("127.0.0.1", 3005);
    do {
        client->processSocketEvent();
        std::this_thread::sleep_for(std::chrono::milliseconds(3));
    } while(true);
    logger::log("shutdown client");
    return 0;
}
