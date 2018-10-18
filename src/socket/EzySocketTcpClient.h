#pragma once

#include "EzySocketClient.h"

EZY_NAMESPACE_START_WITH(socket)

#ifdef USE_WINSOCK_2
	typedef SOCKET EzyTcpSocket;
#else 
	typedef int EzyTcpSocket;
#endif

class EzySocketTcpWriter : public EzySocketWriter {
public:
	EzyTcpSocket mSocket;
protected:
	virtual void update();
public:
	EzySocketTcpWriter();
	virtual ~EzySocketTcpWriter();
};

class EzySocketTcpReader : public EzySocketReader {
	virtual void update();
public:
	EzyTcpSocket mSocket;
public:
	EzySocketTcpReader();
	virtual ~EzySocketTcpReader();
};

class EzySocketTcpClient : public EzySocketClient {
	EzyTcpSocket mSocket;
	std::mutex mSocketMutex;

	virtual void resetSocket();
	virtual void createAdapter();
	virtual bool connectThread();
	virtual void startAdapter();
public:
	EzySocketTcpClient();
	virtual ~EzySocketTcpClient();

	virtual void closeSocket();
};

EZY_NAMESPACE_END_WITH
