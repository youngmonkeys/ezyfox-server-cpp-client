#pragma once

#include <map>
#include "constant/EzyCommand.h"
#include "socket/EzySender.h"
#include "constant/EzyConnectionStatus.h"

EZY_NAMESPACE_START_WITH_ONLY(entity)
class EzyZone;
class EzyUser;
class EzyApp;
EZY_NAMESPACE_END_WITH

EZY_NAMESPACE_START_WITH_ONLY(request)
class EzyRequestSerializer;
EZY_NAMESPACE_END_WITH

EZY_NAMESPACE_START_WITH_ONLY(manager)
class EzyHandlerManager;
class EzyPingManager;
EZY_NAMESPACE_END_WITH

EZY_NAMESPACE_START_WITH_ONLY(socket)
class EzySocketClient;
class EzyPingSchedule;
EZY_NAMESPACE_END_WITH

EZY_NAMESPACE_START_WITH_ONLY(command)
class EzySetup;
EZY_NAMESPACE_END_WITH

EZY_NAMESPACE_START

class EzyClient : public socket::EzySender {
protected:
    command::EzySetup* mSetup;
    socket::EzySocketClient* mSocketClient;
    std::map<int, entity::EzyApp*> mAppsById;
    request::EzyRequestSerializer* mRequestSerializer;
    
    
protected:
    EZY_SYNTHESIZE(entity::EzyUser*, Me)
    EZY_SYNTHESIZE(entity::EzyZone*, Zone)
    EZY_SYNTHESIZE(constant::EzyConnectionStatus, Status)
    EZY_SYNTHESIZE_READONLY(manager::EzyPingManager*, PingManager);
    EZY_SYNTHESIZE_READONLY(socket::EzyPingSchedule*, PingSchedule);
    EZY_SYNTHESIZE_READONLY(manager::EzyHandlerManager*, HandlerManager);
    
protected:
    socket::EzySocketClient* newSocketClient();
    
public:
    EzyClient();
    ~EzyClient();
    void connect(std::string host, int port);
    void disconnect();
    command::EzySetup* setup();
    void send(request::EzyRequest* request);
    void addApp(entity::EzyApp* app);
    entity::EzyApp* getAppById(int appId);
    void processEvents();
};

EZY_NAMESPACE_END
