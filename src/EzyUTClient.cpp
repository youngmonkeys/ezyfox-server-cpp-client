//
//  EzyUTClient.cpp
//  ezyfox-server-cpp-client
//
//  Created by Dung Ta Van on 3/24/20.
//  Copyright © 2020 Young Monkeys. All rights reserved.
//

#include "EzyUTClient.h"
#include "entity/EzyArray.h"
#include "config/EzyClientConfig.h"
#include "request/EzyRequestSerializer.h"
#include "socket/EzyUTSocketClient.h"

EZY_NAMESPACE_START

EzyUTClient::EzyUTClient() {
}

EzyUTClient* EzyUTClient::create(config::EzyClientConfig *config) {
    auto pRet = new EzyUTClient();
    pRet->init(config);
    return pRet;
}

EzyUTClient::~EzyUTClient() {
}

void EzyUTClient::udpConnect(int port) {
    ((socket::EzyUTSocketClient*)mSocketClient)->udpConnectTo(port);
}

void EzyUTClient::udpConnect(std::string host, int port) {
    ((socket::EzyUTSocketClient*)mSocketClient)->udpConnectTo(host, port);
}

void EzyUTClient::udpSend(request::EzyRequest *request, bool encrypted) {
    auto cmd = request->getCommand();
    auto data = request->serialize();
    udpSend(cmd, data, encrypted);
}

void EzyUTClient::udpSend(constant::EzyCommand cmd, entity::EzyArray *data, bool encrypted) {
    auto array = mRequestSerializer->serialize(cmd, data);
    if(mSocketClient) {
        ((socket::EzyUTSocketClient*)mSocketClient)->udpSendMessage(array, encrypted);
        printSentData(cmd, data);
    }
}

socket::EzySocketClient* EzyUTClient::newTcpSocketClient() {
    return new socket::EzyUTSocketClient();
}

EZY_NAMESPACE_END
