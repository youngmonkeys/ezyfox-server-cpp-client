#pragma once

#include "EzySocketWriter.h"
#include "EzySocketReader.h"

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
    std::mutex mClientMutex;
    
    gc::EzyReleasePool* mReleasePool;
    EzySocketWriter* mSocketWriter;
    EzySocketReader* mSocketReader;
    EzySocketEventQueue mSocketEventQueue;
    handler::EzyEventHandlers* mEventHandlers;
    handler::EzyDataHandlers* mDataHandlers;
    manager::EzyHandlerManager* mHandlerManager;
    std::vector<event::EzyEvent*> mStatusBuffer;
    
protected:
    virtual void processEvent();
    virtual void processRecvMessage();
    virtual void clearAdapter();
    virtual void resetSocket();
    virtual void updateConnection();
    virtual void startAdapter();
    virtual void createAdapter();
    virtual bool connectNow();
    virtual void processSocketError();
public:
    EzySocketClient();
    virtual ~EzySocketClient();
    virtual void connectTo(const std::string& host, int port);
    virtual void closeClient();
    virtual void closeSocket();
    virtual void sendMessage(EzySocketData* data);
    virtual void processMessage();
    virtual void setHandlerManager(manager::EzyHandlerManager* handlerManager);
};

EZY_NAMESPACE_END_WITH
