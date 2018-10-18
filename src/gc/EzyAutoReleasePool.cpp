#include <thread>
#include "EzyAutoReleasePool.h"

EZY_NAMESPACE_START_WITH(gc)

EzyAutoReleasePool::EzyAutoReleasePool() {
}

EzyAutoReleasePool::~EzyAutoReleasePool() {
}

EzyReleasePool* EzyAutoReleasePool::getPool() {
    std::unique_lock<std::mutex> lk(mPoolMutex);
    
    size_t threadId = std::hash<std::thread::id>()(std::this_thread::get_id());
    auto it = mPools.find(threadId);
    EzyReleasePool* pret = 0;
    if (it != mPools.end()) {
        pret = it->second;
    }
    else{
        pret = new EzyReleasePool();
        mPools.insert(std::make_pair(threadId, pret));
    }
    
    return pret;
}

void EzyAutoReleasePool::removePool() {
    std::unique_lock<std::mutex> lk(mPoolMutex);
    size_t threadId = std::hash<std::thread::id>()(std::this_thread::get_id());
    auto it = mPools.find(threadId);
    if (it != mPools.end()){
        delete it->second;
        mPools.erase(it);
    }
}

static EzyAutoReleasePool sInstance;
EzyAutoReleasePool* EzyAutoReleasePool::getInstance() {
    return &sInstance;
}

EZY_NAMESPACE_END_WITH
