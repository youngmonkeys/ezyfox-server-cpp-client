//
//  EzyUdpSocketClient.cpp
//  ezyfox-server-cpp-client
//
//  Created by Dung Ta Van on 3/15/20.
//  Copyright © 2020 Young Monkeys. All rights reserved.
//

#include <thread>
#include <chrono>
#include "EzyUdpSocketClient.h"
#include "../logger/EzyLogger.h"
#include "../gc/EzyAutoReleasePool.h"
#include "../concurrent/EzyThread.h"
#include "../config/EzyClientConfig.h"
#include "../codec/EzyDataEncoder.h"
#include "../constant/EzyConnectionFailedReason.h"

EZY_NAMESPACE_START_WITH(socket)

EzyUdpSocketWriter::EzyUdpSocketWriter(config::EzySocketConfig* config)
: EzySocketWriter(config) {
}

EzyUdpSocketWriter::~EzyUdpSocketWriter() {
}

void EzyUdpSocketWriter::update() {
    concurrent::EzyThread::setCurrentThreadName("ezyfox-udp-writer");
    size_t sentData;
#ifdef EZY_DEBUG
    auto releasePool = gc::EzyAutoReleasePool::getInstance()->newPool("udp-writer");
#else
    auto releasePool = gc::EzyAutoReleasePool::getInstance()->getPool();
#endif
    while (true) {
        releasePool->releaseAll();
        if (!isActive()) {
            return;
        }
        
        EzySocketData* sendData = mSocketPool->take();
        if (sendData) {
            sentData = 0;
            toBufferData(sendData);
            const std::vector<char>& sendBuffer = mEncoder->getBuffer();
            send(mSocket, sendBuffer.data() + sentData, sendBuffer.size() - sentData, 0);
        }
        else {
            setActive(false);
            return;
        }
    }
}

/*****************************************/
EzyUdpSocketReader::EzyUdpSocketReader(config::EzySocketConfig* config)
: EzySocketReader(config) {
}

EzyUdpSocketReader::~EzyUdpSocketReader() {
}

#ifdef USE_MESSAGE_HEADER
void EzyUdpSocketReader::preInsertData() {
    mDecodeState = codec::prepareMessage;
    mByteBuffer.clear();
}
#endif

void EzyUdpSocketReader::update() {
    concurrent::EzyThread::setCurrentThreadName("ezyfox-udp-reader");
    size_t rs = 0;
    char dataBuffer[mBufferSize];
#ifdef EZY_DEBUG
    auto releasePool = gc::EzyAutoReleasePool::getInstance()->newPool("udp-reader");
#else
    auto releasePool = gc::EzyAutoReleasePool::getInstance()->getPool();
#endif
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(3));
        releasePool->releaseAll();
        
        if (!isActive()) {
            break;
        }
        rs = recv(mSocket, dataBuffer, mBufferSize, 0);
        if (rs > 0) {
            acceptData(dataBuffer, rs);
        }
    }
}

/*****************************************/
EzyUdpSocketClient::EzyUdpSocketClient() {
    mSocketReader = 0;
    mSocketWriter = 0;
    mSocket = SYS_SOCKET_INVALID;
    mHandshakeEncoder = new codec::EzyDataEncoder(1024);
    mSocketStatuses = new util::EzyStack<EzySocketStatus>(SocketNotConnect);
#ifdef USE_WINSOCK_2
    WORD wVersionRequested;
    WSADATA wsaData;
    wVersionRequested = MAKEWORD(2, 2);
    WSAStartup(wVersionRequested, &wsaData);
#endif
}

EzyUdpSocketClient::~EzyUdpSocketClient() {
    EZY_SAFE_DELETE(mHandshakeEncoder);
#ifdef USE_WINSOCK_2
    WSACleanup();
#endif
}

void EzyUdpSocketClient::connectTo(std::string host, int port) {
    auto status = mSocketStatuses->last();
    if(!isSocketConnectable(status)) {
        logger::log("socket is connecting...");
        return;
    }
    mHost = host;
    mPort = port;
    connect0();
}

void EzyUdpSocketClient::connect0() {
    clearAdapters();
    createAdapters();
    closeSocket();
    mSocketStatuses->clear();
    connectNow();
    startAdapters();
    mSocketStatuses->push(SocketConnecting);
    mConnectionFailedReason = constant::UnknownFailure;
    sendHandshakeRequest();
    retain();
    std::thread newThread(&EzyUdpSocketClient::reconnectDelay, this);
    newThread.detach();
}

void EzyUdpSocketClient::reconnectDelay() {
    concurrent::EzyThread::setCurrentThreadName("ezyfox-udp-reconnect");
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    auto status = mSocketStatuses->last();
    if(status == SocketConnecting)
        mSocketStatuses->push(SocketConnectFailed);
    reconnect();
    release();
}

bool EzyUdpSocketClient::reconnect() {
    auto status = mSocketStatuses->last();
    if (status != SocketConnectFailed) {
        return false;
    }
    logger::log("udp socket is re-connecting...");
    connect0();
    return true;
}

bool EzyUdpSocketClient::connectNow() {
    addrinfo hints, *peer;
    memset(&hints, 0, sizeof(struct addrinfo));
#ifdef __linux
#if defined(ANDROID)
    hints.ai_flags = AI_PASSIVE;
#else
    hints.ai_flags = AI_ALL;
#endif
#else
    hints.ai_flags = AI_ALL;
#endif
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    
    char service[128];
    sprintf(service, "%d", mPort);
    if (int ret = getaddrinfo(mHost.c_str(), service, &hints, &peer) != 0) {
#ifdef EZY_DEBUG
        logger::log("getaddrinfo failure %d", ret);
#endif
        mConnectionFailedReason = constant::UnknownFailure;
        freeaddrinfo(peer);
        return false;
    }
    
    for (auto tpeer = peer; tpeer; tpeer = tpeer->ai_next) {
        mSocket = ::socket(tpeer->ai_family, tpeer->ai_socktype, tpeer->ai_protocol);
        if (mSocket == SYS_SOCKET_INVALID) {
#ifdef EZY_DEBUG
            logger::log("create socket failure");
#endif
            continue;
        }
        
        int rs = connect(mSocket, tpeer->ai_addr, tpeer->ai_addrlen);
        if (rs == 0) {
            freeaddrinfo(peer);
            return true;
        }
#ifdef USE_WINSOCK_2
        closesocket(mSocket);
#else
        close(mSocket);
#endif
    }
    
    freeaddrinfo(peer);
    mConnectionFailedReason = constant::UnknownFailure;
    return false;
}

void EzyUdpSocketClient::createAdapters() {
    mSocketReader = new EzyUdpSocketReader(mConfig);
    mSocketWriter = new EzyUdpSocketWriter(mConfig);
}

void EzyUdpSocketClient::startAdapters() {
    ((EzyUdpSocketWriter*)mSocketWriter)->mSocket = mSocket;
    mSocketWriter->start();
    ((EzyUdpSocketReader*)mSocketReader)->mSocket = mSocket;
    mSocketReader->start();
}

void EzyUdpSocketClient::clearAdapters() {
    clearAdapter(mSocketReader);
    clearAdapter(mSocketWriter);
    mSocketReader = 0;
    mSocketWriter = 0;
}

void EzyUdpSocketClient::clearAdapter(EzySocketAdapter* adapter) {
    if(adapter) {
        adapter->stop();
        adapter->release();
    }
}

void EzyUdpSocketClient::resetSocket() {
    std::unique_lock<std::mutex> lk(mSocketMutex);
    mSocket = SYS_SOCKET_INVALID;
}

void EzyUdpSocketClient::closeSocket() {
    std::unique_lock<std::mutex> lk(mSocketMutex);
    if (mSocket != SYS_SOCKET_INVALID) {
#ifdef USE_WINSOCK_2
        shutdown(mSocket, SD_BOTH);
        closesocket(mSocket);
#else
        shutdown(mSocket, SHUT_RDWR);
        close(mSocket);
#endif
        mSocket = SYS_SOCKET_INVALID;
    }
}

void EzyUdpSocketClient::disconnect(int reason) {
    closeSocket();
    clearAdapters();
    mSocketStatuses->push(SocketDisconnected);
}

void EzyUdpSocketClient::sendMessage(EzySocketData* message) {
    mSocketWriter->offerMessage(message);
}

void EzyUdpSocketClient::setStatus(EzySocketStatus status) {
    mSocketStatuses->push(status);
}

void EzyUdpSocketClient::popReadMessages(std::vector<EzySocketData *> &buffer) {
    auto status = mSocketStatuses->last();
    if(status == SocketConnecting || status == SocketConnected)
        mSocketReader->popMessages(buffer);
}

void EzyUdpSocketClient::sendHandshakeRequest() {
    mHandshakeEncoder->clear();
    int16_t tokenSize = mSessionToken.length();
    int messageSize = 0;
    messageSize += 8; // sessionIdSize
    messageSize += 2; // tokenLengthSize
    messageSize += tokenSize; // messageSize
    char header = 0;
    header |= 1 << 5;
    mHandshakeEncoder->writeNativeInt8(header);
    mHandshakeEncoder->writeNativeInt16(messageSize);
    mHandshakeEncoder->writeNativeInt64(mSessionId);
    mHandshakeEncoder->writeNativeInt16(tokenSize);
    mHandshakeEncoder->writeNativeBytes(mSessionToken.c_str(), tokenSize);
    std::vector<char> buffer = mHandshakeEncoder->getBuffer();
    send(mSocket, buffer.data(), buffer.size() , 0);
}

EZY_NAMESPACE_END_WITH

