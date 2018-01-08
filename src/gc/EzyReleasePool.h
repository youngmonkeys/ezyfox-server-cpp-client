#pragma once

#include "../EzyMacro.h"
#include "../base/EzyRef.h"

EZY_NAMESPACE_START
namespace gc {

class EzyReleasePool {
    std::vector<base::EzyRef*> mObjects;
public:
    EzyReleasePool();
    virtual ~EzyReleasePool();
    virtual void addObject(base::EzyRef* object);
    virtual void releaseAll();
};

}
EZY_NAMESPACE_END
