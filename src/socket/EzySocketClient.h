#pragma once

#include <set>
#include "EzySocketWriter.h"
#include "EzySocketReader.h"

EZY_NAMESPACE_START_WITH_ONLY(config)
class EzyReconnectConfig;
EZY_NAMESPACE_END_WITH

EZY_NAMESPACE_START_WITH_ONLY(event)
class EzyEvent;
EZY_NAMESPACE_END_WITH

EZY_NAMESPACE_START_WITH_ONLY(manager)
class EzyHandlerManager;
EZY_NAMESPACE_END_WITH

EZY_NAMESPACE_START_WITH_ONLY(handler)
class EzyEventHandlers;
class EzyDataHandlers;
EZY_NAMESPACE_END_WITH

EZY_NAMESPACE_START_WITH(socket)

class EzySocketClient : public base::EzyRef {
protected:
    std::string mHost;
    int mPort;
    long long mConnectTime;
    int mReconnectCount;
    int mConnectionFailedReason;
    std::mutex mClientMutex;
    
    gc::EzyReleasePool* mReleasePool;
    EzySocketWriter* mSocketWriter;
    EzySocketReader* mSocketReader;
    EzySocketEventQueue* mSocketEventQueue;
    handler::EzyEventHandlers* mEventHandlers;
    handler::EzyDataHandlers* mDataHandlers;
    manager::EzyHandlerManager* mHandlerManager;
    std::vector<event::EzyEvent*> mLocalEventQueue;
protected:
    EZY_SYNTHESIZE_WRITEONLY(std::set<int>, UnloggableCommands);
    EZY_SYNTHESIZE_WRITEONLY(config::EzyReconnectConfig*, ReconnectConfig);
protected:
    virtual bool connectNow();
    virtual void connect0(long sleepTime);
    virtual void connect1(long sleepTime);
    virtual void processEvents();
    virtual void clearAdapters();
    virtual void resetSocket();
    virtual void startAdapters();
    virtual void createAdapters();
    virtual void stopAdapter(EzySocketAdapter* adapter);
    virtual void clearAdapter(EzySocketAdapter* adapter);
    virtual void processReceivedMessages();
    virtual void processReceivedMessage(EzySocketData* message);
protected:
    virtual void printReceivedData(int cmd, entity::EzyArray* data);
public:
    EzySocketClient();
    virtual ~EzySocketClient();
    virtual void connectTo(const std::string& host, int port);
    virtual bool reconnect();
    virtual void closeClient();
    virtual void closeSocket();
    virtual void sendMessage(EzySocketData* message);
    virtual void processEventMessages();
    virtual void onDisconnected(int reason);
    virtual void setHandlerManager(manager::EzyHandlerManager* handlerManager);
};

EZY_NAMESPACE_END_WITH
