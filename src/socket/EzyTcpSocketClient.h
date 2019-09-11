#ifndef EzyTcpSocketClient_h
#define EzyTcpSocketClient_h

#include "EzySocketClient.h"

EZY_NAMESPACE_START_WITH_ONLY(config)
class EzySocketConfig;
EZY_NAMESPACE_END_WITH

EZY_NAMESPACE_START_WITH(socket)

#ifdef USE_WINSOCK_2
	typedef SOCKET EzyTcpSocket;
#else 
	typedef int EzyTcpSocket;
#endif

class EzyTcpSocketWriter : public EzySocketWriter {
public:
	EzyTcpSocket mSocket;
protected:
	virtual void update();
public:
	EzyTcpSocketWriter(config::EzySocketConfig* config);
	virtual ~EzyTcpSocketWriter();
};

class EzyTcpSocketReader : public EzySocketReader {
protected:
	virtual void update();
public:
	EzyTcpSocket mSocket;
public:
	EzyTcpSocketReader(config::EzySocketConfig* config);
	virtual ~EzyTcpSocketReader();
};

class EzyTcpSocketClient : public EzySocketClient {
protected:
	EzyTcpSocket mSocket;
	std::mutex mSocketMutex;

	virtual void resetSocket();
	virtual void createAdapters();
	virtual bool connectNow();
	virtual void startAdapters();
public:
	EzyTcpSocketClient();
	virtual ~EzyTcpSocketClient();

	virtual void closeSocket();
};

EZY_NAMESPACE_END_WITH

#endif /* EzyTcpSocketClient_h */
