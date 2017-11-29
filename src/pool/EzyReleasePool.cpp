//
//  EzyReleasePool.cpp
//  ezyfox-server-cpp-client
//
//  Created by Dung Ta Van on 11/28/17.
//  Copyright © 2017 Young Monkeys. All rights reserved.
//

#include "EzyReleasePool.h"

EZY_NAMESPACE_START
namespace pool {
    
EzyReleasePool::EzyReleasePool() {
    
}
    
EzyReleasePool::~EzyReleasePool() {
    this->releaseAll();
}
    
void EzyReleasePool::addObject(entity::EzyRef* obj) {
    objects.push_back(obj);
    obj->retain();
}
    
void EzyReleasePool::releaseAll(){
    for (int i = 0; i < objects.size(); i++) {
        objects[i]->release();
    }
    objects.clear();
}
    
}
EZY_NAMESPACE_END
