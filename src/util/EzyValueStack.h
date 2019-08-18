//
//  EzyValueList.h
//  ezyfox-server-cpp-client
//
//  Created by Dung Ta Van on 8/13/19.
//  Copyright © 2019 Young Monkeys. All rights reserved.
//

#pragma once

#include <mutex>
#include <vector>
#include "../EzyMacro.h"

EZY_NAMESPACE_START_WITH(util)

template <class V>
class EzyValueStack {
protected:
    std::mutex mStackMutex;
    V mTopValue;
    V mLastValue;
    V mDefaultValue;
    std::vector<V> mValues;
public:
    EzyValueStack(V defValue = 0);
    ~EzyValueStack();
    V top();
    V last();
    V pop();
    void popAll(std::vector<V>& buffer);
    void push(V value);
    void clear();
    void purgeAll();
    int size();
};

template <class V>
EzyValueStack<V>::EzyValueStack(V defaultValue) {
    mTopValue = defaultValue;
    mLastValue = defaultValue;
    mDefaultValue = defaultValue;
    mValues.clear();
}

template <class V>
EzyValueStack<V>::~EzyValueStack<V>() {
    mValues.clear();
}

template <class V>
V EzyValueStack<V>::top() {
    std::unique_lock<std::mutex> lk(mStackMutex);
    return mTopValue;
}

template <class V>
V EzyValueStack<V>::last() {
    std::unique_lock<std::mutex> lk(mStackMutex);
    return mLastValue;
}

template <class V>
V EzyValueStack<V>::pop() {
    std::unique_lock<std::mutex> lk(mStackMutex);
    size_t size = mValues.size();
    if(size > 0) {
        mTopValue = mValues.at(size - 1);
        mValues.pop_back();
    }
    else {
        mTopValue = mDefaultValue;
    }
    return mTopValue;
}

template <class V>
void EzyValueStack<V>::popAll(std::vector<V> &buffer) {
    std::unique_lock<std::mutex> lk(mStackMutex);
    for(int i = (int)mValues.size() - 1 ; i >= 0 ; --i) {
        buffer.push_back(mValues[i]);
    }
    mValues.clear();
}

template <class V>
void EzyValueStack<V>::push(V value) {
    std::unique_lock<std::mutex> lk(mStackMutex);
    mTopValue = value;
    mLastValue = value;
    mValues.push_back(value);
}

template <class V>
void EzyValueStack<V>::clear() {
    std::unique_lock<std::mutex> lk(mStackMutex);
    mTopValue = mDefaultValue;
    mLastValue = mDefaultValue;
    mValues.clear();
}

template <class V>
void EzyValueStack<V>::purgeAll() {
    std::unique_lock<std::mutex> lk(mStackMutex);
    for (int i = 0 ; i < mValues.size() ; ++i) {
        delete mValues[i];
    }
    mTopValue = mDefaultValue;
    mLastValue = mDefaultValue;
}

template <class V>
int EzyValueStack<V>::size() {
    std::unique_lock<std::mutex> lk(mStackMutex);
    int size = mValues.size();
    return size;
}

EZY_NAMESPACE_END_WITH
