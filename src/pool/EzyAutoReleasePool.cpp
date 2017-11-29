//
//  EzyAutoReleasePool.cpp
//  ezyfox-server-cpp-client
//
//  Created by Dung Ta Van on 11/28/17.
//  Copyright © 2017 Young Monkeys. All rights reserved.
//

#include <thread>
#include "EzyAutoReleasePool.h"

EZY_NAMESPACE_START
namespace pool {

EzyAutoReleasePool::EzyAutoReleasePool() {
}

EzyAutoReleasePool::~EzyAutoReleasePool() {
    // TODO Auto-generated destructor stub
}

EzyReleasePool* EzyAutoReleasePool::getPool() {
    std::unique_lock<std::mutex> lk(poolMutex);
    
    size_t threadId = std::hash<std::thread::id>()(std::this_thread::get_id());
    //quyetnd::log("thread: %d", threadId);
    auto it = pools.find(threadId);
    EzyReleasePool* pret = 0;
    if (it != pools.end()) {
        pret = it->second;
    }
    else{
        pret = new EzyReleasePool();
        pools.insert(std::make_pair(threadId, pret));
    }
    
    return pret;
}

void EzyAutoReleasePool::removePool() {
    std::unique_lock<std::mutex> lk(poolMutex);
    size_t threadId = std::hash<std::thread::id>()(std::this_thread::get_id());
    auto it = pools.find(threadId);
    if (it != pools.end()){
        delete it->second;
        pools.erase(it);
    }
}

static EzyAutoReleasePool instance;
EzyAutoReleasePool* EzyAutoReleasePool::getInstance() {
    return &instance;
}
    
}
EZY_NAMESPACE_END
