//
//  EzyUdpSocketClient.h
//  ezyfox-server-cpp-client
//
//  Created by Dung Ta Van on 3/15/20.
//  Copyright © 2020 Young Monkeys. All rights reserved.
//

#ifndef EzyUdpSocketClient_h
#define EzyUdpSocketClient_h

#include "EzySocketClient.h"

EZY_NAMESPACE_START_WITH_ONLY(config)
class EzySocketConfig;
EZY_NAMESPACE_END_WITH

EZY_NAMESPACE_START_WITH_ONLY(codec)
class EzyDataEncoder;
EZY_NAMESPACE_END_WITH

EZY_NAMESPACE_START_WITH(socket)

#ifdef USE_WINSOCK_2
typedef SOCKET EzyUdpSocket;
#else
typedef int EzyUdpSocket;
#endif

class EzyUdpSocketWriter : public EzySocketWriter {
public:
    EzyUdpSocket mSocket;
protected:
    virtual void update();
public:
    EzyUdpSocketWriter(config::EzySocketConfig* config);
    virtual ~EzyUdpSocketWriter();
};

class EzyUdpSocketReader : public EzySocketReader {
protected:
#ifdef USE_MESSAGE_HEADER
    virtual void preInsertData();
#endif
    virtual void update();
public:
    EzyUdpSocket mSocket;
public:
    EzyUdpSocketReader(config::EzySocketConfig* config);
    virtual ~EzyUdpSocketReader();
};

class EzyUdpSocketClient : public base::EzyRef {
protected:
    EzyUdpSocket mSocket;
    std::mutex mSocketMutex;
    int mConnectionFailedReason;
    EzySocketWriter* mSocketWriter;
    EzySocketReader* mSocketReader;
    codec::EzyDataEncoder* mHandshakeEncoder;
    util::EzyStack<EzySocketStatus>* mSocketStatuses;
protected:
    EZY_SYNTHESIZE_READONLY(std::string, Host);
    EZY_SYNTHESIZE_READONLY(int, Port);
    EZY_SYNTHESIZE(int64_t, SessionId);
    EZY_SYNTHESIZE(std::string, SessionToken);
    EZY_SYNTHESIZE_WRITEONLY(config::EzySocketConfig*, Config);
protected:
    virtual void connect0();
    virtual void resetSocket();
    virtual void clearAdapters();
    virtual void createAdapters();
    virtual bool connectNow();
    virtual void reconnectDelay();
    virtual void startAdapters();
    virtual void clearAdapter(EzySocketAdapter* adapter);
    virtual void sendHandshakeRequest();
public:
    EzyUdpSocketClient();
    virtual ~EzyUdpSocketClient();
    virtual void connectTo(std::string host, int port);
    virtual bool reconnect();
    virtual void sendMessage(EzySocketData* message, bool encrypted);
    virtual void setStatus(EzySocketStatus status);
    virtual void closeSocket();
    virtual void disconnect(int reason);
    virtual void popReadMessages(std::vector<EzySocketData*>& buffer);
};

EZY_NAMESPACE_END_WITH

#endif /* EzyUdpSocketClient_h */
