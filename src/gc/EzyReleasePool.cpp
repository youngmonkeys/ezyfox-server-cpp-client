#include "../base/EzyRef.h"
#include "EzyReleasePool.h"

EZY_NAMESPACE_START_WITH(gc)

#ifdef EZY_DEBUG
EzyReleasePool::EzyReleasePool(std::string name) {
    this->mName = name;
}
#endif
    
EzyReleasePool::EzyReleasePool() {
}
    
EzyReleasePool::~EzyReleasePool() {
    this->releaseAll();
}
    
void EzyReleasePool::addObject(base::EzyRef* object) {
    mObjects.push_back(object);
    object->retain();
}
    
void EzyReleasePool::releaseAll() {
    for (int i = 0; i < mObjects.size(); ++i) {
        mObjects[i]->release();
    }
    mObjects.clear();
}

EZY_NAMESPACE_END_WITH
