//
//  EzyUTSocketClient.cpp
//  ezyfox-server-cpp-client
//
//  Created by Dung Ta Van on 3/24/20.
//  Copyright © 2020 Young Monkeys. All rights reserved.
//

#include "EzyUTSocketClient.h"
#include "EzyUdpSocketClient.h"

EZY_NAMESPACE_START_WITH(socket)

EzyUTSocketClient::EzyUTSocketClient() {
    mUdpClient = new EzyUdpSocketClient();
}

EzyUTSocketClient::~EzyUTSocketClient() {
    EZY_SAFE_RELEASE(mUdpClient);
}

void EzyUTSocketClient::udpConnectTo(int port) {
    udpConnectTo(mHost, port);
}

void EzyUTSocketClient::udpConnectTo(std::string host, int port) {
    mUdpClient->setSessionId(mSessionId);
    mUdpClient->setSessionToken(mSessionToken);
    mUdpClient->connectTo(host, port);
}

void EzyUTSocketClient::udpSendMessage(EzySocketData* message, bool encrypted) {
    mUdpClient->sendMessage(message, encrypted);
}

void EzyUTSocketClient::udpSetStatus(EzySocketStatus status) {
    mUdpClient->setStatus(status);
}

void EzyUTSocketClient::setConfig(config::EzySocketConfig *config) {
    EzySocketClient::setConfig(config);
    mUdpClient->setConfig(config);
}

void EzyUTSocketClient::popReadMessages() {
    EzyTcpSocketClient::popReadMessages();
    mUdpClient->popReadMessages(mLocalMessageQueue);
}

void EzyUTSocketClient::clearComponents(int disconnectReason) {
    mUdpClient->disconnect(disconnectReason);
}

EZY_NAMESPACE_END_WITH
