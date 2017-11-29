#pragma once

#include "../EzyMacro.h"
#include "../entity/EzyRef.h"

EZY_NAMESPACE_START
namespace pool {

class EzyReleasePool {
    std::vector<entity::EzyRef*> objects;
public:
    EzyReleasePool();
    virtual ~EzyReleasePool();
    virtual void addObject(entity::EzyRef* obj);
    virtual void releaseAll();
};

}
EZY_NAMESPACE_END
