//
//  EzyUTClient.h
//  ezyfox-server-cpp-client
//
//  Created by Dung Ta Van on 3/24/20.
//  Copyright © 2020 Young Monkeys. All rights reserved.
//

#ifndef EzyUTClient_h
#define EzyUTClient_h

#include "EzyClient.h"

EZY_NAMESPACE_START

class EzyUTClient : public EzyClient {
public:
    static EzyUTClient* create(config::EzyClientConfig* config);
public:
    EzyUTClient();
    virtual ~EzyUTClient();
    virtual void udpConnect(int port);
    virtual void udpConnect(std::string host, int port);
    virtual void udpSend(request::EzyRequest* request, bool encrypted = false);
    virtual void udpSend(constant::EzyCommand cmd, entity::EzyArray* data, bool encrypted = false);
protected:
    virtual socket::EzySocketClient* newTcpSocketClient();
};

EZY_NAMESPACE_END

#endif /* EzyUTClient_h */
