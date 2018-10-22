#include "EzySocketPool.h"

EZY_NAMESPACE_START_WITH(socket)

EzySocketPool::EzySocketPool() {
    mDataQueue = new std::queue<EzySocketData*>();
}

EzySocketPool::~EzySocketPool() {
    if (mDataQueue) {
        while (!mDataQueue->empty()) {
            EzySocketData* data = mDataQueue->front();
            if (data) {
                //delete data;
                data->release();
            }
            mDataQueue->pop();
        }
        
        delete mDataQueue;
        mDataQueue = 0;
    }
    
}

void EzySocketPool::push(EzySocketData* data) {
    std::unique_lock<std::mutex> lk(mPoolMutex);
    if (mDataQueue) {
        data->retain();
        mDataQueue->push(data);
    }
    
    mPoolCondition.notify_one();
}

void EzySocketPool::clear() {
    std::unique_lock<std::mutex> lk(mPoolMutex);
    if (mDataQueue) {
        while (!mDataQueue->empty()) {
            EzySocketData* data = mDataQueue->front();
            //delete data;
            data->release();
            mDataQueue->pop();
        }
    }
    
    mPoolCondition.notify_all();
}

EzySocketData* EzySocketPool::take() {
    std::unique_lock<std::mutex> lk(mPoolMutex);
    if (mDataQueue) {
        if (!mDataQueue->empty()) {
            EzySocketData* data = mDataQueue->front();
            mDataQueue->pop();
            
            //data->retain();
            data->autorelease();
            return data;
        }
        
        mPoolCondition.wait(lk);
        
        if (mDataQueue && !mDataQueue->empty()) {
            EzySocketData* data = mDataQueue->front();
            mDataQueue->pop();
            
            //data->retain();
            data->autorelease();
            return data;
        }
        else{
            return 0;
        }
    }
    
    return 0;
}

EzySocketData* EzySocketPool::pop() {
    std::unique_lock<std::mutex> lk(mPoolMutex);
    if (mDataQueue && !mDataQueue->empty()) {
        auto data = mDataQueue->front();
        mDataQueue->pop();
        
        //data->retain();
        data->autorelease();
        return data;
    }
    return 0;
}

EZY_NAMESPACE_END_WITH
