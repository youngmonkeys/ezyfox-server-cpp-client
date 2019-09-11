#ifndef EzyReleasePool_h
#define EzyReleasePool_h

#include <vector>
#include "../EzyMacro.h"

#ifdef EZY_DEBUG
#include <string>
#endif

EZY_NAMESPACE_START_WITH_ONLY(base)
class EzyRef;
EZY_NAMESPACE_END_WITH

EZY_NAMESPACE_START_WITH(gc)

class EzyReleasePool {
protected:
    std::vector<base::EzyRef*> mObjects;
protected:
#ifdef EZY_DEBUG
    EZY_SYNTHESIZE_READONLY(std::string, Name)
#endif
public:
#ifdef EZY_DEBUG
    EzyReleasePool(std::string name);
#endif
public:
    EzyReleasePool();
    virtual ~EzyReleasePool();
    virtual void addObject(base::EzyRef* object);
    virtual void releaseAll();
};

EZY_NAMESPACE_END_WITH

#endif /* EzyReleasePool_h */
