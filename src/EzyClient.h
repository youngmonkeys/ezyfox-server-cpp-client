#ifndef EzyClient_h
#define EzyClient_h

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
class EzyPlugin;
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

EZY_NAMESPACE_START_WITH_ONLY(setup)
class EzySetup;
EZY_NAMESPACE_END_WITH

EZY_NAMESPACE_START

class EzyClient : public socket::EzySender {
protected:
    setup::EzySetup* mSetup;
    std::set<int> mUnloggableCommands;
    socket::EzySocketClient* mSocketClient;
    request::EzyRequestSerializer* mRequestSerializer;
protected:
    EZY_SYNTHESIZE(entity::EzyUser*, Me)
    EZY_SYNTHESIZE(entity::EzyZone*, Zone)
    EZY_SYNTHESIZE(constant::EzyConnectionStatus, Status);
    EZY_SYNTHESIZE(constant::EzyConnectionStatus, UdpStatus);
    EZY_SYNTHESIZE_READONLY(std::string, Name);
    EZY_SYNTHESIZE_READONLY(int64_t, SessionId);
    EZY_SYNTHESIZE_READONLY(std::string, SessionToken);
    EZY_SYNTHESIZE_READONLY(std::string, SessionKey);
    EZY_SYNTHESIZE_READONLY(config::EzyClientConfig*, Config);
    EZY_SYNTHESIZE_READONLY(manager::EzyPingManager*, PingManager);
    EZY_SYNTHESIZE_READONLY(socket::EzyPingSchedule*, PingSchedule);
    EZY_SYNTHESIZE_READONLY(manager::EzyHandlerManager*, HandlerManager);
protected:
    void preconnect();
    socket::EzySocketClient* newSocketClient();
    void printSentData(int cmd, entity::EzyArray* data);
protected:
    virtual void init(config::EzyClientConfig* config);
    virtual socket::EzySocketClient* newTcpSocketClient();
public:
    static EzyClient* create(config::EzyClientConfig* config);
public:
    EzyClient();
    virtual ~EzyClient();
    setup::EzySetup* setup();
    void connect(std::string host, int port);
    bool reconnect();
    void disconnect(int reason = -1);
    void send(request::EzyRequest* request, bool encrypted = false);
    void send(constant::EzyCommand cmd, entity::EzyArray* data, bool encrypted = false);
    void processEvents();
    entity::EzyApp* getApp();
    entity::EzyApp* getAppById(int appId);
    entity::EzyPlugin* getPluginById(int pluginId);
    socket::EzySocketClient* getSocket();
    void setSessionId(int64_t sessionId);
    void setSessionToken(std::string token);
    void setSessionKey(std::string sessionKey);
    void destroy();
    bool isConnected();
    bool isUdpConnected();
    bool isEnableSSL();
public:
    virtual void udpConnect(int port);
    virtual void udpConnect(std::string host, int port);
    virtual void udpSend(request::EzyRequest* request, bool encrypted = false);
    virtual void udpSend(constant::EzyCommand cmd, entity::EzyArray* data, bool encrypted = false);
};

EZY_NAMESPACE_END

#endif /* EzyClient_h */
