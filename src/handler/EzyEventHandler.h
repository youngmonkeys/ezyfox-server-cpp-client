#pragma one

#include "../EzyMacro.h"
#include "../event/EzyEvent.h"

EZY_NAMESPACE_START_WITH(handler)

class EzyEventHandler {
public:
    virtual ~EzyEventHandler();
    virtual void handle(event::EzyEvent* event) = 0;
};

template <class T>
class EzyAbstractEventHandler : public EzyEventHandler {
public:
    void handle(event::EzyEvent* event);
    virtual void process(T *event) = 0;
};

template <class T>
void EzyAbstractEventHandler<T>::handle(event::EzyEvent* event) {
    this->process((T*)event);
}

EZY_NAMESPACE_END_WITH
