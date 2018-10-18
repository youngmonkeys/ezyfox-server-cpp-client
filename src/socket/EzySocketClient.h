#pragma once

#include "EzySocketWriter.h"
#include "EzySocketReader.h"

EZY_NAMESPACE_START_WITH(socket)

class EzySocketClient : public base::EzyRef {
protected:
    std::string mHost;
    int mPort;
    long long mConnectTime;
    
    std::mutex mClientMutex;
    
    EzySocketWriter* mSocketWriter;
    EzySocketReader* mSocketReader;
    EzySocketClientStatus mClientStatus;
    
    gc::EzyReleasePool* mReleasePool;
    
    std::vector<EzySocketStatusData> mStatusBuffer;
    
    EzySocketDataHandler *mDataHandler;
    EzySocketStatusHandler *mStatusHandler;
    
    virtual void processEvent();
    virtual void processRecvMessage();
    virtual void clearAdapter();
    virtual void resetSocket();
    virtual void updateConnection();
    virtual void startAdapter();
    
    virtual void createAdapter();
    virtual bool connectThread();
    virtual void processSocketError();
public:
    EzySocketClient();
    virtual ~EzySocketClient();
    
    virtual void connectTo(const std::string& host, int port);
    
    virtual void closeClient();
    virtual void closeSocket();
    
    virtual EzySocketStatusType getStatus();
    virtual void setStatus(EzySocketStatusType status, bool event = true);
    
    virtual void sendMessage(EzySocketData* data);
    
    virtual void processMessage();
    
    virtual void setDataHandler(EzySocketDataHandler* handler);
    virtual void setStatusHandler(EzySocketStatusHandler* handler);
};

EZY_NAMESPACE_END_WITH
