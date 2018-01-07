#pragma once

#include "EzyHeaders.h"
#include "constant/EzyCommand.h"
#include "handler/EzyEventHandlers.h"
#include "request/EzyRequest.h"
#include "request/EzyRequestDeliver.h"
#include "request/EzyArrayRequestSerializer.h"

EZY_NAMESPACE_START

class EzyClient : public request::EzyRequestDeliver {
protected:
    socket::EzySocketClient* mSocketClient;
    handler::EzyEventHandlers* mEventHandlers;
    request::EzyArrayRequestSerializer* mRequestSerializer;
protected:
    socket::EzySocketClient* newSocketClient();
    
public:
    EzyClient();
    ~EzyClient();

    template<class T>
    void addEventHandler(event::EzyEventType eventType,
                         handler::EzyEventHandler<T>* handler);
    void handleEvent(event::EzyEvent* event);
    
    void connect(std::string host, int port);
    void disconnect();
    void send(request::EzyRequest* request);
    void processSocketEvent();
    
};

EZY_NAMESPACE_END
