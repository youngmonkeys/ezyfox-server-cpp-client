#pragma once

#include "EzyHeaders.h"
#include "constant/EzyCommand.h"
#include "handler/EzyEventHandlers.h"
#include "request/EzyRequest.h"
#include "socket/EzySender.h"
#include "request/EzyArrayRequestSerializer.h"
#include "EzySocketKeyPair.h"
#include "socket/EzyPingSchedule.h"
#include "EzyApp.h"
#include "EzyPlugin.h"

EZY_NAMESPACE_START_WITH_ONLY(entity)
class EzyZone;
class EzyUser;
class EzyApp;
EZY_NAMESPACE_END_WITH

EZY_NAMESPACE_START

class EzyClient : public socket::EzySender {
protected:
    socket::EzySocketClient* mSocketClient;
    handler::EzyEventHandlers* mEventHandlers;
    request::EzyArrayRequestSerializer* mRequestSerializer;
    
protected:
    EZY_SYNTHESIZE(entity::EzyUser*, Me)
    EZY_SYNTHESIZE(entity::EzyZone*, Zone)
    
protected:
    EZY_SYNTHESIZE_BOOL_READONLY(Connected)
    
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
    void addApp(entity::EzyApp* app);
    entity::EzyApp* getAppById(int appId);
    manager::EzyPingManager* getPingManager();
    void processSocketEvent();
    
};

template<class T>
void EzyClient::addEventHandler(event::EzyEventType eventType,
                                handler::EzyEventHandler<T> *handler) {
    mEventHandlers->addEventHandler(eventType, handler);
}

EZY_NAMESPACE_END
