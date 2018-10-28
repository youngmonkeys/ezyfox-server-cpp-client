#include "EzySocketPool.h"

EZY_NAMESPACE_START_WITH(socket)

EzySocketPool::EzySocketPool() {
    clear0();
}

EzySocketPool::~EzySocketPool() {
    clear0();
}

void EzySocketPool::push(EzySocketData* data) {
    {
        std::unique_lock<std::mutex> lk(mPoolMutex);
        mDataQueue.push(data);
    }
    mPoolCondition.notify_one();
}

void EzySocketPool::clear() {
    std::unique_lock<std::mutex> lk(mPoolMutex);
    clear0();
    mPoolCondition.notify_all();
}

void EzySocketPool::clear0() {
    while (!mDataQueue.empty()) {
        EzySocketData* data = mDataQueue.front();
        EZY_SAFE_DELETE(data);
        mDataQueue.pop();
    }
}

EzySocketData* EzySocketPool::take() {
    std::unique_lock<std::mutex> lk(mPoolMutex);
    mPoolCondition.wait(lk, [=] { return !this->mDataQueue.empty(); });
    if(mDataQueue.empty()) return 0;
    EzySocketData* data = mDataQueue.front();
    mDataQueue.pop();
    return data;
}

EzySocketData* EzySocketPool::pop() {
    std::unique_lock<std::mutex> lk(mPoolMutex);
    if (!mDataQueue.empty()) {
        auto data = mDataQueue.front();
        mDataQueue.pop();
        data->autorelease();
        return data;
    }
    return 0;
}

void EzySocketPool::popAll(std::vector<EzySocketData*>& buffer) {
    std::unique_lock<std::mutex> lk(mPoolMutex);
    while (!mDataQueue.empty()) {
        auto data = mDataQueue.front();
        mDataQueue.pop();
        data->autorelease();
        buffer.push_back(data);
    }
}

EZY_NAMESPACE_END_WITH
