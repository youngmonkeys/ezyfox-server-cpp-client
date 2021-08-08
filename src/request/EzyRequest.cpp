#include "EzyRequest.h"
#include "../entity/EzyNull.h"

#define EZY_IMPLEMENT_REQUEST_CLASS(className) \
Ezy##className##Request::Ezy##className##Request() : EzyRequest() {\
}\
Ezy##className##Request* Ezy##className##Request::create() {\
    Ezy##className##Request* pRet = new Ezy##className##Request();\
    pRet->autorelease();\
    return pRet;\
}\
constant::EzyCommand Ezy##className##Request::getCommand() {\
    return constant::className;\
}

#define EZY_IMPLEMENT_REQUEST_CLASS_NO_CONSTRUCTOR(className) \
Ezy##className##Request* Ezy##className##Request::create() {\
    Ezy##className##Request* pRet = new Ezy##className##Request();\
    pRet->autorelease();\
    return pRet;\
}\
constant::EzyCommand Ezy##className##Request::getCommand() {\
    return constant::className;\
}

EZY_NAMESPACE_START_WITH(request)

//=======================================================

EZY_IMPLEMENT_REQUEST_CLASS(Ping)

entity::EzyArray* EzyRequest::serialize() {
    return new entity::EzyArray();
}

//=======================================================

entity::EzyArray* EzyHandshakeRequest::serialize() {
    auto array = new entity::EzyArray();
    array->addString(mClientId);
    array->addString(mClientKey);
    array->addString(mClientType);
    array->addString(mClientVersion);
    array->addBool(mEnableEncryption);
    array->addString(mToken);
    return array;
}

EZY_IMPLEMENT_REQUEST_CLASS(Handshake)

//=======================================================

EzyLoginRequest::EzyLoginRequest() {
    this->mPassword = "";
    this->mData = 0;
}

entity::EzyArray* EzyLoginRequest::serialize() {
    auto array = new entity::EzyArray();
    array->addString(mZoneName);
    array->addString(mUsername);
    array->addString(mPassword);
    array->addArray(mData);
    return array;
}

EZY_IMPLEMENT_REQUEST_CLASS_NO_CONSTRUCTOR(Login)

//=======================================================

EzyAppAccessRequest::EzyAppAccessRequest() {
    this->mData = 0;
}

entity::EzyArray* EzyAppAccessRequest::serialize() {
    auto array = new entity::EzyArray();
    array->addString(mAppName);
    array->addItem(mData ? mData : new entity::EzyNull());
    return array;
}

EZY_IMPLEMENT_REQUEST_CLASS_NO_CONSTRUCTOR(AppAccess)

//=======================================================

EzyAppRequestRequest::EzyAppRequestRequest() {
    this->mData = 0;
}

entity::EzyArray* EzyAppRequestRequest::serialize() {
    auto array = new entity::EzyArray();
    array->addInt(mAppId);
    array->addItem(mData);
    return array;
}

EZY_IMPLEMENT_REQUEST_CLASS_NO_CONSTRUCTOR(AppRequest)

//=======================================================

entity::EzyArray* EzyAppExitRequest::serialize() {
    auto array = new entity::EzyArray();
    array->addInt(mAppId);
    return array;
}

EZY_IMPLEMENT_REQUEST_CLASS_NO_CONSTRUCTOR(AppExit)

//=======================================================

EzyPluginRequestRequest::EzyPluginRequestRequest() {
    this->mData = 0;
}

entity::EzyArray* EzyPluginRequestRequest::serialize() {
    auto array = new entity::EzyArray();
    array->addInt(mPluginId);
    array->addItem(mData);
    return array;
}

EZY_IMPLEMENT_REQUEST_CLASS_NO_CONSTRUCTOR(PluginRequest)

EZY_NAMESPACE_END_WITH
