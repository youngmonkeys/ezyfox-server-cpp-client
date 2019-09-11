//
//  EzyValueList.h
//  ezyfox-server-cpp-client
//
//  Created by Dung Ta Van on 8/13/19.
//  Copyright © 2019 Young Monkeys. All rights reserved.
//

#ifndef EzyStack_h
#define EzyStack_h

#include <mutex>
#include <vector>
#include "../EzyMacro.h"

EZY_NAMESPACE_START_WITH(util)

template <class V>
class EzyStack {
protected:
    std::mutex mStackMutex;
    V mTopValue;
    V mLastValue;
    V mDefaultValue;
    std::vector<V> mValues;
public:
    EzyStack(V defValue = 0);
    ~EzyStack();
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
EzyStack<V>::EzyStack(V defaultValue) {
    mTopValue = defaultValue;
    mLastValue = defaultValue;
    mDefaultValue = defaultValue;
    mValues.clear();
}

template <class V>
EzyStack<V>::~EzyStack<V>() {
    mValues.clear();
}

template <class V>
V EzyStack<V>::top() {
    std::unique_lock<std::mutex> lk(mStackMutex);
    return mTopValue;
}

template <class V>
V EzyStack<V>::last() {
    std::unique_lock<std::mutex> lk(mStackMutex);
    return mLastValue;
}

template <class V>
V EzyStack<V>::pop() {
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
void EzyStack<V>::popAll(std::vector<V> &buffer) {
    std::unique_lock<std::mutex> lk(mStackMutex);
    for(int i = (int)mValues.size() - 1 ; i >= 0 ; --i) {
        buffer.push_back(mValues[i]);
    }
    mValues.clear();
}

template <class V>
void EzyStack<V>::push(V value) {
    std::unique_lock<std::mutex> lk(mStackMutex);
    mTopValue = value;
    mLastValue = value;
    mValues.push_back(value);
}

template <class V>
void EzyStack<V>::clear() {
    std::unique_lock<std::mutex> lk(mStackMutex);
    mTopValue = mDefaultValue;
    mLastValue = mDefaultValue;
    mValues.clear();
}

template <class V>
void EzyStack<V>::purgeAll() {
    std::unique_lock<std::mutex> lk(mStackMutex);
    for (int i = 0 ; i < mValues.size() ; ++i) {
        delete mValues[i];
    }
    mTopValue = mDefaultValue;
    mLastValue = mDefaultValue;
}

template <class V>
int EzyStack<V>::size() {
    std::unique_lock<std::mutex> lk(mStackMutex);
    int stackSize = mValues.size();
    return stackSize;
}

EZY_NAMESPACE_END_WITH

#endif /* EzyStack_h */
