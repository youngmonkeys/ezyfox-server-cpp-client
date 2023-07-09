//
//  EzyUTSocketClient.h
//  ezyfox-server-cpp-client
//
//  Created by Dung Ta Van on 3/24/20.
//  Copyright © 2020 Young Monkeys. All rights reserved.
//

#ifndef EzyUTSocketClient_h
#define EzyUTSocketClient_h

#include "EzyTcpSocketClient.h"

EZY_NAMESPACE_START_WITH(socket)

class EzyUdpSocketClient;

class EzyUTSocketClient : public EzyTcpSocketClient {
protected:
    EzyUdpSocketClient* mUdpClient;
public:
    EzyUTSocketClient();
    virtual ~EzyUTSocketClient();
    virtual void udpConnectTo(int port);
    virtual void udpConnectTo(std::string host, int port);
    virtual void udpSendMessage(EzySocketData* message, bool encrypted);
    virtual void udpSetStatus(EzySocketStatus status);
    virtual void setConfig(config::EzySocketConfig* config);
protected:
    virtual void popReadMessages();
    virtual void clearComponents(int disconnectReason);
};

EZY_NAMESPACE_END_WITH

#endif /* EzyUTSocketClient_h */
