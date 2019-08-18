#pragma once

#include <map>
#include <set>
#include <string>
#include "constant/EzyCommand.h"
#include "socket/EzySender.h"
#include "constant/EzyConnectionStatus.h"

EZY_NAMESPACE_START_WITH_ONLY(config)
class EzyClientConfig;
EZY_NAMESPACE_END_WITH

EZY_NAMESPACE_START_WITH_ONLY(entity)
class EzyZone;
class EzyUser;
class EzyApp;
class EzyArray;
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
    std::set<int> mUnloggableCommands;
    socket::EzySocketClient* mSocketClient;
    request::EzyRequestSerializer* mRequestSerializer;
protected:
    EZY_SYNTHESIZE(entity::EzyUser*, Me)
    EZY_SYNTHESIZE(entity::EzyZone*, Zone)
    EZY_SYNTHESIZE(constant::EzyConnectionStatus, Status);
    EZY_SYNTHESIZE_READONLY(std::string, Name);
    EZY_SYNTHESIZE_READONLY(config::EzyClientConfig*, Config);
    EZY_SYNTHESIZE_READONLY(manager::EzyPingManager*, PingManager);
    EZY_SYNTHESIZE_READONLY(socket::EzyPingSchedule*, PingSchedule);
    EZY_SYNTHESIZE_READONLY(manager::EzyHandlerManager*, HandlerManager);
protected:
    void preconnect();
    socket::EzySocketClient* newSocketClient();
    void printSentData(int cmd, entity::EzyArray* data);
public:
    EzyClient(config::EzyClientConfig* config);
    ~EzyClient();
    command::EzySetup* setup();
    void connect(std::string host, int port);
    bool reconnect();
    void disconnect(int reason);
    void send(request::EzyRequest* request);
    void send(constant::EzyCommand cmd, entity::EzyArray* data);
    void processEvents();
    entity::EzyApp* getAppById(int appId);
    void destroy();
};

EZY_NAMESPACE_END
