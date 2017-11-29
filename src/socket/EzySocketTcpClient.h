#pragma once

#include "EzySocketAdapter.h"

EZY_NAMESPACE_START
namespace socket {

#ifdef USE_WINSOCK_2
	typedef SOCKET EzyTcpSocket;
#else 
	typedef int EzyTcpSocket;
#endif

class EzySocketTcpWriter : public EzySocketWriter {
public:
	EzyTcpSocket socketClient;
protected:
	virtual void update();
public:
	EzySocketTcpWriter();
	virtual ~EzySocketTcpWriter();
};

class EzySocketTcpReader : public EzySocketReader {
	virtual void update();
public:
	EzyTcpSocket socketClient;
public:
	EzySocketTcpReader();
	virtual ~EzySocketTcpReader();
};

class EzySocketTcpClient : public EzySocketClient {
	EzyTcpSocket socketClient;
	std::mutex socketMutex;

	virtual void resetSocket();
	virtual void createAdapter();
	virtual bool connectThread();
	virtual void startAdapter();
public:
	EzySocketTcpClient();
	virtual ~EzySocketTcpClient();

	virtual void closeSocket();
};


}
EZY_NAMESPACE_END
