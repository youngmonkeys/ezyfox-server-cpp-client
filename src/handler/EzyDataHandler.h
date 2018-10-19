#pragma once

#include "../EzyMacro.h"
#include "../entity/EzyValue.h"

EZY_NAMESPACE_START_WITH(handler)

class EzyDataHandler {
public:
    ~EzyDataHandler();
    void handle(entity::EzyValue* data);
};

template <class T>
class EzyAbstractDataHandler : public EzyDataHandler {
public:
    void handle(entity::EzyValue* data);
    void process(T* data);
    
};

template <class T>
void EzyAbstractDataHandler<T>::handle(entity::EzyValue* data) {
    this->process((T*)data);
}

EZY_NAMESPACE_END_WITH
