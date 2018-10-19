#pragma once

#include "../EzyMacro.h"
#include "../entity/EzyValue.h"

EZY_NAMESPACE_START_WITH(handler)

class EzyAppDataHandler {
public:
    virtual ~EzyAppDataHandler();
    virtual void handle(entity::EzyValue* data) = 0;
};

template <class T>
class EzyAbstractAppDataHandler : public EzyAppDataHandler {
protected:
    virtual void process(T* data) = 0;
public:
    void handle(entity::EzyValue* data);
    
};

template <class T>
void EzyAbstractAppDataHandler<T>::handle(entity::EzyValue* data) {
    this->process((T*)data);
}

EZY_NAMESPACE_END_WITH
