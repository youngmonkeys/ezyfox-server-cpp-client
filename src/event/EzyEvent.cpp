#include "EzyEvent.h"

#define EZY_IMPLEMENT_EVENT_CLASS(className, eventType)\
Ezy##className##Event* Ezy##className##Event::create() {\
    Ezy##className##Event* pRet = new Ezy##className##Event();\
    pRet->autorelease();\
    return pRet;\
}\
EzyEventType Ezy##className##Event::getType() {\
    return eventType;\
}

#define EZY_IMPLEMENT_ARGS_EVENT_CLASS(className, eventType)\
Ezy##className##Event::Ezy##className##Event(Ezy##className##EventArgs* args) {\
    this->mArgs = args;\
}\
Ezy##className##Event::~Ezy##className##Event() {\
    EZY_SAFE_DELETE(mArgs);\
}\
Ezy##className##Event* Ezy##className##Event::create(Ezy##className##EventArgs* args) {\
    auto pRet = new Ezy##className##Event(args);\
    pRet->autorelease();\
    return pRet;\
}\
EzyEventType Ezy##className##Event::getType() {\
    return eventType;\
}

#define EZY_CREATE_EVENT_ARGS_METHOD(methodName)\
Ezy##methodName##EventArgs* Ezy##methodName##EventArgs::create(entity::EzyArray *array) {\
    auto pRet = new Ezy##methodName##EventArgs();\
    pRet->deserialize(array);\
    return pRet;\
}

EZY_NAMESPACE_START
namespace event {
//===============================================
    
EZY_IMPLEMENT_EVENT_CLASS(ConnectionSuccess, ConnectionSuccess);
    
//===============================================

EZY_CREATE_EVENT_ARGS_METHOD(Handshake);
    
void EzyHandshakeEventArgs::deserialize(entity::EzyArray *array) {
    mServerPublicKey = array->getString(0);
    mReconnectToken = array->getString(1);
    mReconnect = array->getBool(2);
}
    
EZY_IMPLEMENT_ARGS_EVENT_CLASS(Handshake, Handshake);

//===============================================
    
EZY_CREATE_EVENT_ARGS_METHOD(Login)

void EzyLoginEventArgs::deserialize(entity::EzyArray *array) {
    mUserId = array->getInt(0);
    mUsername = array->getString(1);
    mJoinedApp = array->getArray(2);
    mdata = array->getItem(3);
}
    
EZY_IMPLEMENT_ARGS_EVENT_CLASS(Login, LoginSuccess)
    
}
EZY_NAMESPACE_END
