```cpp
#include <mutex>
#include <vector>
#include <map>
#include "../EzyMacro.h"

EZY_NAMESPACE_START
namespace entity {

inline bool __checkDoubleIsFloat(double d);

enum EzyCommand {
    Error = 10,
    Handshake = 11,
    Ping = 12,
    Pong = 13,
    Disconnect = 14,
    PluginRequest = 15,
    Login = 20,
    LoginError = 21,
    Logout = 22,
    AppAccess = 30,
    AppRequest = 31,
    AppJoined = 32,
    AppExit = 33,
    AppAccessError = 34
};
    
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
```
