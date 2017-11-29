#pragma once

#include <queue>
#include <ctime>
#include <condition_variable>
#include "EzySocketCore.h"
#include "../entity/EzyRef.h"
#include "../codec/EzyDataDecoder.h"
#include "../pool/EzyReleasePool.h"

EZY_NAMESPACE_START
namespace socket {

typedef std::function<void(EzySocketData*)> EzyReceiverCallback;
typedef std::function<void(const EzySocketStatusData& data)> EzySocketStatusCallback;

class EzySocketPool {
    
protected:
    
    std::mutex poolMutex;
	std::condition_variable poolCondition;
    std::queue<EzySocketData*>* socketDataQueue;
    
public:
    
	EzySocketPool();
	virtual ~EzySocketPool();

	virtual EzySocketData* take();
	virtual EzySocketData* pop();
    virtual void push(EzySocketData* data);
	virtual void clear();
};

class EzySocketAdapter : public entity::EzyRef {
    
protected:
    
	bool running;
	std::mutex mutex;
	EzySocketPool* socketPool;
    
	virtual void update();
    
public:
    
	EzySocketAdapter();
	virtual ~EzySocketAdapter();

	virtual void updateThread();

	virtual bool isRunning();
	virtual void setRunning(bool running);

	virtual void start();
	virtual void stop();

	virtual void pushMessage(EzySocketData* data);
	virtual EzySocketData* popMessage();
};

class EzySocketWriter : public EzySocketAdapter {
protected:
    codec::EzyDataEncoder* encoder;
	void toBufferData(EzySocketData* data);
public:
	EzySocketWriter();
	virtual ~EzySocketWriter();
};

class EzySocketReader : public EzySocketAdapter, public codec::EzyDataDecoderDelegate {
protected:
#ifdef USE_MESSAGE_HEADER
	std::vector<char> byteBuffer;
	int dataSize;
	bool recvHeader;

	virtual void onRecvData();
	virtual void onUpdateDataHeader();
	virtual void onUpdateData();
#endif

    codec::EzyDataDecoder* decoder;
	virtual void recvData(const char* data, int size);
    virtual void onRecvMessage(entity::EzyValue* value);
public:
	EzySocketReader();
	virtual ~EzySocketReader();

	virtual void updateThread();
};

/**/
class EzySocketClient : public entity::EzyRef {
protected:
    pool::EzyReleasePool* releasePool;

	long long connectTime;
	std::string host;
	int port;

	std::mutex clientMutex;

	EzySocketClientStatus clientStatus;
	EzySocketWriter* socketWriter;
	EzySocketReader* socketReader;

	std::vector<EzySocketStatusData> statusBuffer;

	virtual void processEvent();
	virtual void processRecvMessage();
	virtual void clearAdapter();
	virtual void resetSocket();
	virtual void updateConnection();
	virtual void startAdapter();

	virtual void createAdapter();
	virtual bool connectThread();
	virtual void processSocketError();
public:
	EzyReceiverCallback recvCallback;
	EzySocketStatusCallback statusCallback;
public:
	EzySocketClient();
	virtual ~EzySocketClient();

	virtual void connectTo(const std::string& host, int port);

	virtual void closeClient();
	virtual void closeSocket();

	virtual EzySocketStatusType getStatus();
	virtual void setStatus(EzySocketStatusType status, bool event = true);

	virtual void sendMessage(EzySocketData* data);

	virtual void processMessage();
};

}
EZY_NAMESPACE_END
