#include <thread>
#include <chrono>
#include "EzyTcpSocketClient.h"
#include "../logger/EzyLogger.h"
#include "../gc/EzyAutoReleasePool.h"
#include "../constant/EzyConnectionFailedReason.h"
#include "../constant/EzyDisconnectReason.h"

EZY_NAMESPACE_START_WITH(socket)

EzyTcpSocketWriter::EzyTcpSocketWriter() {
}

EzyTcpSocketWriter::~EzyTcpSocketWriter() {
}

void EzyTcpSocketWriter::update() {
	size_t rs;
	size_t sentData;
#ifdef EZY_DEBUG
    auto releasePool = gc::EzyAutoReleasePool::getInstance()->newPool("socket-writer");
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

			while (true) {
				rs = send(mSocket, sendBuffer.data() + sentData, sendBuffer.size() - sentData, 0);
				if (rs > 0) {
					sentData += rs;
					if (sentData < sendBuffer.size()) {
						continue;
					}
					break;
				}
				else if (rs == 0) {
#ifdef EZY_DEBUG
                    logger::log("server shutdown[2]");
#endif
					setActive(false);
					return;
				}
				else{
#ifdef EZY_DEBUG
					logger::log("send error");
#endif
					setActive(false);
					return;
				}
			}
		}
		else{
			setActive(false);
			return;
		}
	}
}

/*****************************************/
EzyTcpSocketReader::EzyTcpSocketReader() {
}

EzyTcpSocketReader::~EzyTcpSocketReader() {
}

#define BUFFER_SIZE 102400
void EzyTcpSocketReader::update() {
	size_t rs = 0;
	char dataBuffer[BUFFER_SIZE];
#ifdef EZY_DEBUG
    auto releasePool = gc::EzyAutoReleasePool::getInstance()->newPool("socket-reader");
#else
    auto releasePool = gc::EzyAutoReleasePool::getInstance()->getPool();
#endif
	while (true) {
		std::this_thread::sleep_for(std::chrono::milliseconds(3));
		releasePool->releaseAll();

		if (!isActive()) {
			break;
		}
		rs = recv(mSocket, dataBuffer, BUFFER_SIZE, 0);
		if (rs > 0) {
			acceptData(dataBuffer, rs);
		}
		else if (rs == 0) {
#ifdef EZY_DEBUG
			logger::log("server shutdown[1]");
#endif
			setActive(false);
			break;
		}
		else {
#ifdef EZY_DEBUG
			logger::log("received header error");
#endif
			setActive(false);
			break;
		}
	}
}

/*****************************************/
EzyTcpSocketClient::EzyTcpSocketClient() {
	mSocket = SYS_SOCKET_INVALID;

#ifdef USE_WINSOCK_2
	WORD wVersionRequested;
	WSADATA wsaData;
	wVersionRequested = MAKEWORD(2, 2);
	WSAStartup(wVersionRequested, &wsaData);
#endif
}

EzyTcpSocketClient::~EzyTcpSocketClient() {
#ifdef USE_WINSOCK_2
	WSACleanup();
#endif
}

void EzyTcpSocketClient::closeSocket() {
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

void EzyTcpSocketClient::resetSocket() {
	std::unique_lock<std::mutex> lk(mSocketMutex);
	mSocket = SYS_SOCKET_INVALID;
}

void EzyTcpSocketClient::createAdapters() {
	std::unique_lock<std::mutex> lk(mClientMutex);
	mSocketReader = new EzyTcpSocketReader();
	mSocketWriter = new EzyTcpSocketWriter();
}

void EzyTcpSocketClient::startAdapters() {
	std::unique_lock<std::mutex> lk(mClientMutex);
	((EzyTcpSocketWriter*)mSocketWriter)->mSocket = mSocket;
	mSocketWriter->start();
	((EzyTcpSocketReader*)mSocketReader)->mSocket = mSocket;
	mSocketReader->start();
}


bool EzyTcpSocketClient::connectNow() {
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
	hints.ai_socktype = SOCK_STREAM;

	char service[128];
	sprintf(service, "%d", mPort);
	if (int ret = getaddrinfo(mHost.c_str(), service, &hints, &peer) != 0) {
#ifdef EZY_DEBUG
		logger::log("getaddrinfo failure %d", ret);
#endif
        mConnectionFailedReason = constant::UnknownFailure;
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

EZY_NAMESPACE_END_WITH
