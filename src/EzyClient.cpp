#include <thread>
#include <chrono>
#include "EzyClient.h"
#include "EzySocketKeyPair.h"
#include "handler/EzyPingSchedule.h"

EZY_NAMESPACE_START

//==========================================================

class SocketDataHandler : public socket::EzySocketDataHandler {
protected:
    EZY_SYNTHESIZE_WRITEONLY(handler::EzyEventHandlers*, EventHandlers)
public:
    virtual void handleSocketData(socket::EzySocketData *data);
};

//==========================================================

class SocketStatusHandler : public socket::EzySocketStatusHandler {
protected:
    EZY_SYNTHESIZE_WRITEONLY(handler::EzyEventHandlers*, EventHandlers)
public:
    virtual void handleSocketStatus(const socket::EzySocketStatusData& status);
};

//==========================================================

class ConnectionSuccessEventHandler :
public handler::EzyEventHandler<event::EzyConnectionSuccessEvent> {
protected:
    request::EzyRequestDeliver *requestDeliver;
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
    request::EzyRequestDeliver *requestDeliver;
protected:
    virtual void sendLoginRequest();
public:
    HandshakeEventHandler(request::EzyRequestDeliver *requestDeliver);
    virtual void handle(event::EzyHandshakeEvent *event);
};

//==========================================================

void SocketDataHandler::handleSocketData(socket::EzySocketData *data) {
    entity::EzyArray *array = (entity::EzyArray*)data;
    constant::EzyCommand cmd = (constant::EzyCommand)array->getInt(0);
    switch (cmd) {
        case constant::handshake: {
            event::EzyHandshakeEvent* event = event::EzyHandshakeEvent::create();
            mEventHandlers->handleEvent(event);
        }
            break;
        case constant::login: {
            
            break;
        }
        default:
            break;
    }
}

//==========================================================

void SocketStatusHandler::handleSocketStatus(const socket::EzySocketStatusData &status) {
    switch (status.status) {
        case socket::Connected: {
            event::EzyConnectionSuccessEvent *event = event::EzyConnectionSuccessEvent::create();
            mEventHandlers->handleEvent(event);
        }
            break;
        default:
            break;
    }
}

//==========================================================

void ConnectionSuccessEventHandler::handle(event::EzyConnectionSuccessEvent *event) {
    sendHandShakeRequest();
}

void ConnectionSuccessEventHandler::sendHandShakeRequest() {
    auto *keyPairGentor = new socket::EzyRsaKeyPairGentor();
    auto *keyPair = keyPairGentor->generate(512);
    auto clientKey = keyPair->getPublicKey();
    logger::log("public key: %s", clientKey.c_str());
    auto params = new request::EzyHandshakeParams();
    params->setClientId("clientId");
    params->setClientKey(clientKey);
    params->setReconnectToken("reconectToken");
    auto *request = request::EzyHandshakeRequest::create(params);
    requestDeliver->send(request);
}

ConnectionSuccessEventHandler::
ConnectionSuccessEventHandler(request::EzyRequestDeliver *requestDeliver) {
    this->requestDeliver = requestDeliver;
}

//==========================================================

HandshakeEventHandler::
HandshakeEventHandler(request::EzyRequestDeliver *requestDeliver) {
    this->requestDeliver = requestDeliver;
}

void HandshakeEventHandler::handle(event::EzyHandshakeEvent *event) {
    sendLoginRequest();
    handler::EzyPingSchedule *pingSchedule = new handler::EzyPingSchedule(requestDeliver);
    pingSchedule->setPeriod(5);
    pingSchedule->start();
}

void HandshakeEventHandler::sendLoginRequest() {
    auto params = new request::EzyLoginParams();
    params->setUsername("dungtv");
    params->setPassword("123456");
    params->setData(new entity::EzyArray());
    auto *request = request::EzyLoginRequest::create(params);
    requestDeliver->send(request);
}

//==========================================================

EzyClient::EzyClient() {
    mEventHandlers = new handler::EzyEventHandlers();
    mRequestSerializer = new request::EzyArrayRequestSerializer();
    addEventHandler(event::connectionSuccess, new ConnectionSuccessEventHandler(this));
    addEventHandler(event::handshake, new HandshakeEventHandler(this));
}

EzyClient::~EzyClient() {
    EZY_SAFE_DELETE(mEventHandlers);
}

template<class T>
void EzyClient::addEventHandler(event::EzyEventType eventType,
                                handler::EzyEventHandler<T> *handler) {
    mEventHandlers->addEventHandler(eventType, handler);
}

void EzyClient::handleEvent(event::EzyEvent *event) {
    mEventHandlers->handleEvent(event);
}

void EzyClient::connect(std::string host, int port) {
    mSocketClient = newSocketClient();
    auto socketDataHandler = new SocketDataHandler();
    socketDataHandler->setEventHandlers(mEventHandlers);
    auto socketStatusHanlder = new SocketStatusHandler();
    socketStatusHanlder->setEventHandlers(mEventHandlers);
    mSocketClient->setDataHandler(socketDataHandler);
    mSocketClient->setStatusHandler(socketStatusHanlder);
    mSocketClient->connectTo(host, port);
}

socket::EzySocketClient* EzyClient::newSocketClient() {
    return new socket::EzySocketTcpClient();
}

void EzyClient::disconnect() {
    if(mSocketClient)
        mSocketClient->closeSocket();
    EZY_SAFE_DELETE(mSocketClient);
}

void EzyClient::processSocketEvent() {
    mSocketClient->processMessage();
}

void EzyClient::send(request::EzyRequest *request) {
    entity::EzyArray* data = mRequestSerializer->serialize(request);
    if(mSocketClient)
        mSocketClient->sendMessage(data);
}

EZY_NAMESPACE_END
