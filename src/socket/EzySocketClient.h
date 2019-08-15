#pragma once

#include <set>
#include "EzySocketWriter.h"
#include "EzySocketReader.h"
#include "EzySocketDelegate.h"
#include "../util/EzyValueStack.h"

EZY_NAMESPACE_START_WITH_ONLY(config)
class EzyReconnectConfig;
EZY_NAMESPACE_END_WITH

EZY_NAMESPACE_START_WITH_ONLY(event)
class EzyEvent;
EZY_NAMESPACE_END_WITH

EZY_NAMESPACE_START_WITH_ONLY(manager)
class EzyHandlerManager;
class EzyPingManager;
EZY_NAMESPACE_END_WITH

EZY_NAMESPACE_START_WITH_ONLY(handler)
class EzyEventHandlers;
class EzyDataHandlers;
EZY_NAMESPACE_END_WITH

EZY_NAMESPACE_START_WITH(socket)

class EzyPingSchedule;

class EzySocketClient : public base::EzyRef, public EzySocketDelegate {
protected:
    long long mConnectTime;
    int mReconnectCount;
    int mDisconnectReason;
    int mConnectionFailedReason;
    
    gc::EzyReleasePool* mReleasePool;
    EzySocketWriter* mSocketWriter;
    EzySocketReader* mSocketReader;
    EzyPingSchedule* mPingSchedule;
    EzySocketEventQueue* mSocketEventQueue;
    handler::EzyEventHandlers* mEventHandlers;
    handler::EzyDataHandlers* mDataHandlers;
    manager::EzyHandlerManager* mHandlerManager;
    std::vector<event::EzyEvent*> mLocalEventQueue;
    std::vector<EzySocketData*> mLocalMessageQueue;
    std::vector<EzySocketStatus> mSocketStatusesBuffer;
    util::EzyValueStack<EzySocketStatus>* mSocketStatuses;
protected:
    EZY_SYNTHESIZE_READONLY(std::string, Host);
    EZY_SYNTHESIZE_READONLY(int, Port);
    EZY_SYNTHESIZE_WRITEONLY(std::set<int>, UnloggableCommands);
    EZY_SYNTHESIZE_WRITEONLY(manager::EzyPingManager*, PingManager);
    EZY_SYNTHESIZE_WRITEONLY(config::EzyReconnectConfig*, ReconnectConfig);
protected:
    virtual bool connectNow();
    virtual void connect0(long sleepTime);
    virtual void connect1(long sleepTime);
    virtual void clearAdapters();
    virtual void resetSocket();
    virtual void startAdapters();
    virtual void createAdapters();
    virtual void clearAdapter(EzySocketAdapter* adapter);
    virtual void processEvents();
    virtual void processStatuses();
    virtual void processReceivedMessages();
    virtual void processReceivedMessages0();
    virtual void processReceivedMessage(EzySocketData* message);
protected:
    virtual void printReceivedData(int cmd, entity::EzyArray* data);
public:
    EzySocketClient();
    virtual ~EzySocketClient();
    virtual void connectTo(const std::string& host, int port);
    virtual bool reconnect();
    virtual void closeSocket();
    virtual void sendMessage(EzySocketData* message);
    virtual void processEventMessages();
    virtual void disconnect(int reason);
    virtual void onDisconnected(int reason);
    virtual void setPingSchedule(EzyPingSchedule* pingSchedule);
    virtual void setHandlerManager(manager::EzyHandlerManager* handlerManager);
};

EZY_NAMESPACE_END_WITH
