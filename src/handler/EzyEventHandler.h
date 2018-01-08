#pragma one

#include "../EzyMacro.h"

EZY_NAMESPACE_START
namespace handler {

template <class T>
class EzyEventHandler {
public:
    virtual ~EzyEventHandler();
    virtual void handle(T *event) = 0;
};

template <class T>
EzyEventHandler<T>::~EzyEventHandler() {
}
    
}
EZY_NAMESPACE_END
