//
//  main.cpp
//  ezyfox-server-cpp-client
//
//  Created by Dung Ta Van on 11/28/17.
//  Copyright © 2017 Young Monkeys. All rights reserved.
//

#include <iostream>
#include "EzyClient.h"

#define CLIENT_KEY "MFkwDQYJKoZIhvcNAQEBBQADSAAwRQJAfQmBWNzB2SlezzGGUapMOFQLOJ8fw6PQQutmYHK5rAXSZi893R49W99J7Aufh6t1ib6PxorGH2pc4xKTaBVbbQIBAw=="

using namespace com::tvd12::ezyfoxserver::client;

void sendHandShake(socket::EzySocketTcpClient *client);

class SocketDataHandler : public socket::EzySocketDataHandler {
private:
    socket::EzySocketTcpClient *client;
public:
    SocketDataHandler(socket::EzySocketTcpClient *client);
    virtual void handleSocketData(socket::EzySocketData* data);
};

class SocketStatusHandler : public socket::EzySocketStatusHandler {
private:
    socket::EzySocketTcpClient *client;
public:
    SocketStatusHandler(socket::EzySocketTcpClient *client);
    virtual void handleSocketStatus(const socket::EzySocketStatusData& status);
};

SocketDataHandler::SocketDataHandler(socket::EzySocketTcpClient *client) {
    this->client = client;
}

void SocketDataHandler::handleSocketData(socket::EzySocketData *data) {
}

SocketStatusHandler::SocketStatusHandler(socket::EzySocketTcpClient *client) {
    this->client = client;
}

void SocketStatusHandler::handleSocketStatus(const socket::EzySocketStatusData &status) {
    if(status.status == socket::EzySocketStatusType::Connected) {
    }
    switch (status.status) {
        case socket::Connected:
            break;
        case socket::Connecting:
            sendHandShake(client);
            break;
        default:
            break;
    }
}

void sendHandShake(socket::EzySocketTcpClient *client) {
    auto *params = new entity::EzyArray();
    params->addString("clientId");
    params->addString(CLIENT_KEY);
    params->addString("token");
    params->addString("C++");
    params->addString("0.0.1");
    client->sendMessage(params);
}

//MFkwDQYJKoZIhvcNAQEBBQADSAAwRQJAfQmBWNzB2SlezzGGUapMOFQLOJ8fw6PQQutmYHK5rAXSZi893R49W99J7Aufh6t1ib6PxorGH2pc4xKTaBVbbQIBAw==

int main(int argc, const char * argv[]) {
    socket::EzySocketTcpClient *client = new socket::EzySocketTcpClient();
    logger::log("start client");
    client->setDataHandler(new SocketDataHandler(client));
    client->setStatusHandler(new SocketStatusHandler(client));
    client->connectTo("188.166.213.37", 3005);
    do {
        client->processMessage();
    } while(client->getStatus() != socket::EzySocketStatusType::Closed);
    logger::log("shutdown client");
    return 0;
}
