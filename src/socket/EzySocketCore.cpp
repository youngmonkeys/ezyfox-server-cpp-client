#include <map>
#include "EzySocketCore.h"
#include "../event/EzyEvent.h"

EZY_NAMESPACE_START_WITH(socket)

bool isSocketConnectable(EzySocketStatus status) {
    return status == SocketNotConnect ||
    status == SocketDisconnected ||
    status == SocketConnectFailed;
}

bool isSocketDisconnectable(EzySocketStatus status) {
    return status == SocketConnected || status == SocketDisconnecting;
}

bool isSocketReconnectable(EzySocketStatus status) {
    return status == SocketDisconnected || status == SocketConnectFailed;
}

bool isSocketDestroyable(EzySocketStatus status) {
    return status == SocketNotConnect ||
    status == SocketDisconnected ||
    status == SocketConnectFailed;
}

EzySocketEventQueue::EzySocketEventQueue() {
}

EzySocketEventQueue::~EzySocketEventQueue() {
    clear();
}

void EzySocketEventQueue::addEvent(event::EzyEvent *event) {
    mEvents.push_back(event);
}

void EzySocketEventQueue::popAll(std::vector<event::EzyEvent*> &buffer) {
	std::unique_lock<std::mutex> lk(mMutex);
	for (int i = 0; i < mEvents.size(); ++i) {
        auto event = mEvents[i];
        event->autorelease();
		buffer.push_back(event);
	}
    mEvents.clear();
}

void EzySocketEventQueue::clear() {
	std::unique_lock<std::mutex> lk(mMutex);
    EZY_SAFE_DELETE_VECTOR(mEvents);
}

EZY_NAMESPACE_END_WITH
