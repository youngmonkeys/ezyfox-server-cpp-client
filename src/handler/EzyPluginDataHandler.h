//
//  EzyPluginDataHandler.h
//  ezyfox-server-cpp-client
//
//  Created by Dung Ta Van on 8/19/19.
//  Copyright © 2019 Young Monkeys. All rights reserved.
//

#ifndef EzyPluginDataHandler_h
#define EzyPluginDataHandler_h

#include "../EzyMacro.h"
#include "../entity/EzyValue.h"

EZY_NAMESPACE_START_WITH_ONLY(entity)
class EzyPlugin;
EZY_NAMESPACE_END_WITH

EZY_NAMESPACE_START_WITH(handler)

class EzyPluginDataHandler {
public:
    virtual ~EzyPluginDataHandler();
    virtual void handle(entity::EzyPlugin* app, entity::EzyValue* data) = 0;
};

template <class T>
class EzyAbstractPluginDataHandler : public EzyPluginDataHandler {
protected:
    virtual void process(entity::EzyPlugin* app, T* data) = 0;
public:
    void handle(entity::EzyPlugin* app, entity::EzyValue* data);
    
};

template <class T>
void EzyAbstractPluginDataHandler<T>::handle(entity::EzyPlugin* app, entity::EzyValue* data) {
    this->process(app, (T*)data);
}

EZY_NAMESPACE_END_WITH

#endif /* EzyPluginDataHandler_h */
