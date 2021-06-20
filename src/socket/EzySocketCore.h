#ifndef EzySocketCore_h
#define EzySocketCore_h

#include <mutex>
#include "../EzyMacro.h"
#include "EzySocketDefine.h"
#include "../entity/EzyValue.h"

EZY_NAMESPACE_START_WITH_ONLY(event)
class EzyEvent;
EZY_NAMESPACE_END_WITH

EZY_NAMESPACE_START_WITH(socket)

enum EzySocketStatus {
    SocketNotConnect = 1,
    SocketConnecting,
    SocketConnected,
    SocketConnectFailed,
    SocketDisconnecting,
    SocketDisconnected,
    SocketReconnecting
};

bool isSocketConnectable(EzySocketStatus status);
bool isSocketDisconnectable(EzySocketStatus status);
bool isSocketReconnectable(EzySocketStatus status);
bool isSocketDestroyable(EzySocketStatus status);

typedef entity::EzyValue EzySocketData;

class EzySocketEventQueue {
protected:
	std::mutex mQueueMutex;
    std::vector<event::EzyEvent*> mEvents;
public:
	EzySocketEventQueue();
	~EzySocketEventQueue();

    void addEvent(event::EzyEvent* event);
    void popAll(std::vector<event::EzyEvent*> &buffer);
	void clear();
};

class EzySocketPacket : public base::EzyRef {
protected:
    EzySocketData* mData;
    bool mEncrypted;
public:
    EzySocketPacket(EzySocketData* data, bool encrypted);
    virtual ~EzySocketPacket();
public:
    static EzySocketPacket* create(EzySocketData* data, bool encrypted);
public:
    EzySocketData* getData();
    bool isEncrypted();
};


EZY_NAMESPACE_END_WITH

#endif /* EzySocketCore_h */
