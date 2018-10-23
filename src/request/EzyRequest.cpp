#include "EzyRequest.h"

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
    return entity::EzyArray::create();
}

//=======================================================

entity::EzyArray* EzyHandshakeRequest::serialize() {
    auto array = entity::EzyArray::create();
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
    auto array = entity::EzyArray::create();
    array->addString(mZoneName);
    array->addString(mUsername);
    array->addString(mPassword);
    array->addArray(mData);
    return array;
}

EZY_IMPLEMENT_REQUEST_CLASS_NO_CONSTRUCTOR(Login)

//=======================================================

entity::EzyArray* EzyAppAccessRequest::serialize() {
    auto array = entity::EzyArray::create();
    array->addString(mAppName);
    array->addItem(mData);
    return array;
}

EZY_IMPLEMENT_REQUEST_CLASS(AppAccess)

//=======================================================

EzyAppRequestRequest::EzyAppRequestRequest() {
    this->mData = 0;
}

entity::EzyArray* EzyAppRequestRequest::serialize() {
    auto array = entity::EzyArray::create();
    array->addInt(mAppId);
    array->addItem(mData);
    return array;
}

EZY_IMPLEMENT_REQUEST_CLASS_NO_CONSTRUCTOR(AppRequest)

//=======================================================

EzyPluginRequestByIdRequest::EzyPluginRequestByIdRequest() {
    this->mData = 0;
}

entity::EzyArray* EzyPluginRequestByIdRequest::serialize() {
    auto array = entity::EzyArray::create();
    array->addInt(mPluginId);
    array->addItem(mData);
    return array;
}

EZY_IMPLEMENT_REQUEST_CLASS_NO_CONSTRUCTOR(PluginRequestById)

//=======================================================

EzyPluginRequestByNameRequest::EzyPluginRequestByNameRequest() {
    this->mData = 0;
}

entity::EzyArray* EzyPluginRequestByNameRequest::serialize() {
    auto array = entity::EzyArray::create();
    array->addString(mPluginName);
    array->addItem(mData);
    return array;
}

EZY_IMPLEMENT_REQUEST_CLASS_NO_CONSTRUCTOR(PluginRequestByName)

EZY_NAMESPACE_END_WITH
