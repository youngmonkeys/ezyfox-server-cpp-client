#pragma once

#include <vector>
#include "../EzyMacro.h"

EZY_NAMESPACE_START_WITH_ONLY(base)
class EzyRef;
EZY_NAMESPACE_END_WITH

EZY_NAMESPACE_START_WITH(gc)

class EzyReleasePool {
    std::vector<base::EzyRef*> mObjects;
public:
    EzyReleasePool();
    virtual ~EzyReleasePool();
    virtual void addObject(base::EzyRef* object);
    virtual void releaseAll();
};

EZY_NAMESPACE_END_WITH
