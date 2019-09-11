#ifndef EzyAppDataHandler_h
#define EzyAppDataHandler_h

#include "../EzyMacro.h"
#include "../entity/EzyValue.h"

EZY_NAMESPACE_START_WITH_ONLY(entity)
class EzyApp;
EZY_NAMESPACE_END_WITH

EZY_NAMESPACE_START_WITH(handler)

class EzyAppDataHandler {
public:
    virtual ~EzyAppDataHandler();
    virtual void handle(entity::EzyApp* app, entity::EzyValue* data) = 0;
};

template <class T>
class EzyAbstractAppDataHandler : public EzyAppDataHandler {
protected:
    virtual void process(entity::EzyApp* app, T* data) = 0;
public:
    void handle(entity::EzyApp* app, entity::EzyValue* data);
    
};

template <class T>
void EzyAbstractAppDataHandler<T>::handle(entity::EzyApp* app, entity::EzyValue* data) {
    this->process(app, (T*)data);
}

EZY_NAMESPACE_END_WITH

#endif /* EzyAppDataHandler_h */
