//
//  EzyMainEventsLoop.cpp
//  ezyfox-server-cpp-client
//
//  Created by Dung Ta Van on 8/4/19.
//  Copyright © 2019 Young Monkeys. All rights reserved.
//

#include <thread>
#include <chrono>
#include "../EzyClient.h"
#include "../EzyClients.h"
#include "EzyMainEventsLoop.h"

EZY_NAMESPACE_START_WITH(socket)

EzyMainEventsLoop::EzyMainEventsLoop() {
    this->mActive = false;
    this->mClients = EzyClients::getInstance();
}

EzyMainEventsLoop::~EzyMainEventsLoop() {
    this->mActive = false;
    this->mClients = 0;
    this->mCachedClients.clear();
}

void EzyMainEventsLoop::start(int sleepTime) {
    this->mActive = true;
    while (mActive) {
        processEvents(sleepTime);
    }
}

void EzyMainEventsLoop::processEvents(int sleepTime) {
    std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
    mClients->getClients(mCachedClients);
    for (int i = 0; i < mCachedClients.size(); ++i)
        mCachedClients[i]->processEvents();
}

void EzyMainEventsLoop::stop() {
    this->mActive = false;
}

EZY_NAMESPACE_END_WITH
