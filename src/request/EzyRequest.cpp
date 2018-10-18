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

EZY_NAMESPACE_START_WITH(request)

//=======================================================

EZY_IMPLEMENT_REQUEST_CLASS(Ping)

entity::EzyArray* EzyRequest::serialize() {
    return entity::EzyArray::create();
}

//=======================================================

std::string EzyHandshakeRequest::getClientType() {
    return "C++";
}

std::string EzyHandshakeRequest::getClientVersion() {
    return "1.0.0";
}

entity::EzyArray* EzyHandshakeRequest::serialize() {
    auto array = new entity::EzyArray();
    array->addString(mClientId);
    array->addString(mClientKey);
    array->addString(mToken);
    array->addString(getClientType());
    array->addString(getClientVersion());
    return array;
}

EZY_IMPLEMENT_REQUEST_CLASS(Handshake)

//=======================================================

entity::EzyArray* EzyLoginRequest::serialize() {
    auto array = new entity::EzyArray();
    array->addString(mUsername);
    array->addString(mPassword);
    array->addArray(mData);
    return array;
}

EZY_IMPLEMENT_REQUEST_CLASS(Login)

//=======================================================

entity::EzyArray* EzyAppAccessRequest::serialize() {
    auto array = new entity::EzyArray();
    array->addString(mAppName);
    array->addItem(mData);
    return array;
}

EZY_IMPLEMENT_REQUEST_CLASS(AppAccess)

//=======================================================

entity::EzyArray* EzyAppRequestRequest::serialize() {
    auto array = new entity::EzyArray();
    array->addInt(mAppId);
    array->addItem(mData);
    return array;
}

EZY_IMPLEMENT_REQUEST_CLASS(AppRequest)

//=======================================================

entity::EzyArray* EzyPluginRequestByIdRequest::serialize() {
    auto array = new entity::EzyArray();
    array->addInt(mPluginId);
    array->addItem(mData);
    return array;
}

EZY_IMPLEMENT_REQUEST_CLASS(PluginRequestById)

//=======================================================

entity::EzyArray* EzyPluginRequestByNameRequest::serialize() {
    auto array = new entity::EzyArray();
    array->addString(mPluginName);
    array->addItem(mData);
    return array;
}

EZY_IMPLEMENT_REQUEST_CLASS(PluginRequestByName)

EZY_NAMESPACE_END_WITH
