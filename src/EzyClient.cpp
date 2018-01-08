#include <thread>
#include <chrono>
#include "EzyClient.h"

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
    auto cmd = (command::EzyCommand)array->getInt(0);
    event::EzyEvent* event = nullptr;
    switch (cmd) {
        case command::Handshake: {
            auto args = event::EzyHandshakeEventArgs::create(array->getArray(1));
            event = event::EzyHandshakeEvent::create(args);
        }
            break;
        case command::Login: {
            auto args = event::EzyLoginEventArgs::create(array->getArray(1));
            event = event::EzyLoginEvent::create(args);
            break;
        }
        default:
            break;
    }
    if(event) mEventHandlers->handleEvent(event);
}

//==========================================================

void EzySimpleSocketStatusHandler::handleSocketStatus(const socket::EzySocketStatusData &status) {
    switch (status.status) {
        case socket::Connected: {
            auto *event = event::EzyConnectionSuccessEvent::create();
            mEventHandlers->handleEvent(event);
        }
            break;
        default:
            break;
    }
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

void EzyClient::processSocketEvent() {
    mSocketClient->processMessage();
}

void EzyClient::send(request::EzyRequest *request) {
    auto data = mRequestSerializer->serialize(request);
    if(mSocketClient)
        mSocketClient->sendMessage(data);
}

EZY_NAMESPACE_END
