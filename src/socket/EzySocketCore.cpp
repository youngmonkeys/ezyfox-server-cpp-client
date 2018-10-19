#include <map>
#include "EzySocketCore.h"
#include "../event/EzyEvent.h"

EZY_NAMESPACE_START_WITH(socket)

EzySocketEventQueue::EzySocketEventQueue() {
}

EzySocketEventQueue::~EzySocketEventQueue() {
    mEvents.clear();
}

void EzySocketEventQueue::addEvent(event::EzyEvent *event) {
    mEvents.push_back(event);
}

void EzySocketEventQueue::popAll(std::vector<event::EzyEvent*> &buffer){
	std::unique_lock<std::mutex> lk(mMutex);
	for (int i = 0; i < mEvents.size(); i++){
		buffer.push_back(mEvents[i]);
	}
    mEvents.clear();
}

void EzySocketEventQueue::clear(){
	std::unique_lock<std::mutex> lk(mMutex);
	mEvents.clear();
}

EZY_NAMESPACE_END_WITH
