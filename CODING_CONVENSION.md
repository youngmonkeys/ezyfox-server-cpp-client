#include <mutex>
#include <vector>
#include <map>
#include "../EzyMacro.h"

EZY_NAMESPACE_START
namespace entity {

inline bool __checkDoubleIsFloat(double d);
    
class EzyRef {
    protected:
        int mRetainCount;
        std::mutex mMutex;
        static std::map sMap;
    public:
        EzyRef();
        virtual ~EzyRef();
        virtual void retain();
        virtual void release();
        virtual void autorelease();
        virtual void setRefCount(int count);
    };
    
}
EZY_NAMESPACE_END
