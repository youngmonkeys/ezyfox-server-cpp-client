#include "EzySocketPool.h"

EZY_NAMESPACE_START_WITH(socket)

EzySocketPool::EzySocketPool() {
    mDestroyed = false;
    doClear();
}

EzySocketPool::~EzySocketPool() {
    doClear();
}

void EzySocketPool::push(EzySocketData* data) {
    std::unique_lock<std::mutex> lk(mPoolMutex);
    mDataQueue.push(EzySocketPacket::create(data, false));
}

void EzySocketPool::offer(EzySocketData* data, bool encrypted) {
    std::unique_lock<std::mutex> lk(mPoolMutex);
    mDataQueue.push(EzySocketPacket::create(data, encrypted));
    mPoolCondition.notify_one();
}

void EzySocketPool::clear() {
    std::unique_lock<std::mutex> lk(mPoolMutex);
    doClear();
}

void EzySocketPool::destroy() {
    std::unique_lock<std::mutex> lk(mPoolMutex);
    doClear();
    mDestroyed = true;
    mPoolCondition.notify_all();
}

void EzySocketPool::doClear() {
    while (!mDataQueue.empty()) {
        auto data = mDataQueue.front();
        data->getData()->release();
        data->release();
        mDataQueue.pop();
    }
}

EzySocketPacket* EzySocketPool::take() {
    std::unique_lock<std::mutex> lk(mPoolMutex);
    mPoolCondition.wait(lk, [=] { return mDestroyed || this->mDataQueue.size() > 0; });
    if(mDataQueue.empty()) return 0;
    auto packet = mDataQueue.front();
    packet->autorelease();
    packet->getData()->autorelease();
    mDataQueue.pop();
    return packet;
}

EzySocketData* EzySocketPool::pop() {
    std::unique_lock<std::mutex> lk(mPoolMutex);
    if (!mDataQueue.empty()) {
        auto packet = mDataQueue.front();
        auto data = packet->getData();
        mDataQueue.pop();
        packet->autorelease();
        data->autorelease();
        return data;
    }
    return 0;
}

void EzySocketPool::popAll(std::vector<EzySocketData*>& buffer) {
    std::unique_lock<std::mutex> lk(mPoolMutex);
    while (!mDataQueue.empty()) {
        auto packet = mDataQueue.front();
        mDataQueue.pop();
        packet->autorelease();
        packet->getData()->autorelease();
        buffer.push_back(packet->getData());
    }
}

EZY_NAMESPACE_END_WITH
