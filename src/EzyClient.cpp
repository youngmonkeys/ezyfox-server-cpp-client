#include <thread>
#include <chrono>
#include "EzyClient.h"
#include "logger/EzyLogger.h"

EZY_NAMESPACE_START

//==========================================================

class EzySimpleSocketDataHandler : public socket::EzySocketDataHandler {
protected:
    EZY_SYNTHESIZE_WRITEONLY(handler::EzyEventHandlers*, EventHandlers)
public:
    virtual void handleSocketData(socket::EzySocketData *data);
};

//==========================================================

class EzySimpleSocketStatusHandler : public socket::EzySocketStatusHandler {
protected:
    EZY_SYNTHESIZE_WRITEONLY(handler::EzyEventHandlers*, EventHandlers)
public:
    virtual void handleSocketStatus(const socket::EzySocketStatusData& status);
};

//==========================================================

void EzySimpleSocketDataHandler::handleSocketData(socket::EzySocketData *data) {
    auto array = (entity::EzyArray*)data;
    auto cmd = (constant::EzyCommand)array->getInt(0);
    event::EzyEvent* event = nullptr;
    switch (cmd) {
        case constant::Handshake:
            break;
        case constant::Login:
            break;
        case constant::LoginError:
            break;
        case constant::AppAccess:
            break;
        case constant::AppRequest:
            break;
        case constant::AppAccessError:
            break;
        case constant::AppExit:
            break;
        case constant::Disconnect:
            break;
        case constant::Error:
            break;
        case constant::Ping:
            break;
        case constant::PluginRequestById:
            break;
        case constant::Pong:
            break;
        default:
            break;
    }
    if(event) mEventHandlers->handleEvent(event);
}

//==========================================================

void EzySimpleSocketStatusHandler::handleSocketStatus(const socket::EzySocketStatusData &status) {
    event::EzyEvent* event = nullptr;
    switch (status.status) {
        case socket::Connected:
            event = event::EzyConnectionSuccessEvent::create();
            break;
        case socket::Closed:
            break;
        case socket::LostConnection:
            break;
        default:
            break;
    }
    if(event)
        mEventHandlers->handleEvent(event);
    else
        logger::log("has no handler with status %d", status.status);
}

//==========================================================

EzyClient::EzyClient() {
    mEventHandlers = new handler::EzyEventHandlers();
    mRequestSerializer = new request::EzyArrayRequestSerializer();
}

EzyClient::~EzyClient() {
    EZY_SAFE_DELETE(mEventHandlers);
}

void EzyClient::handleEvent(event::EzyEvent *event) {
    mEventHandlers->handleEvent(event);
}

void EzyClient::connect(std::string host, int port) {
    mSocketClient = newSocketClient();
    auto socketDataHandler = new EzySimpleSocketDataHandler();
    socketDataHandler->setEventHandlers(mEventHandlers);
    auto socketStatusHanlder = new EzySimpleSocketStatusHandler();
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

void EzyClient::processEvents() {
    if(mSocketClient)
        mSocketClient->processMessage();
}

void EzyClient::send(request::EzyRequest *request) {
    auto data = mRequestSerializer->serialize(request);
    if(mSocketClient)
        mSocketClient->sendMessage(data);
}

EZY_NAMESPACE_END
