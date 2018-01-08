#include "EzyRequest.h"

#define EZY_IMPLEMENT_PARAMS_REQUEST_CLASS(className, command) \
Ezy##className##Request::Ezy##className##Request(Ezy##className##RequestParams* params) :\
EzyRequest(command, params) {\
}\
Ezy##className##Request* Ezy##className##Request::create(Ezy##className##RequestParams* params) {\
    Ezy##className##Request* pRet = new Ezy##className##Request(params);\
    pRet->autorelease();\
    return pRet;\
}

#define EZY_IMPLEMENT_REQUEST_CLASS(className, command) \
Ezy##className##Request::Ezy##className##Request() : EzyRequest(command, new EzyParams()) {\
}\
Ezy##className##Request* Ezy##className##Request::create() {\
Ezy##className##Request* pRet = new Ezy##className##Request();\
    pRet->autorelease();\
    return pRet;\
}\

EZY_NAMESPACE_START
namespace request {
    
//=======================================================
EzyParams::~EzyParams() {
}

entity::EzyArray* EzyParams::serialize() {
    return new entity::EzyArray();
}
    
//=======================================================
    
EzyRequest::EzyRequest(command::EzyCommand cmd, EzyParams* params) {
    this->mCommand = cmd;
    this->mParams = params;
}
    
EzyRequest::~EzyRequest() {
    EZY_SAFE_DELETE(mParams);
}
    
EzyRequest* EzyRequest::create(command::EzyCommand cmd, EzyParams* params) {
    auto pRet = new EzyRequest(cmd, params);
    pRet->autorelease();
    return pRet;
}

//=======================================================
    
EZY_IMPLEMENT_REQUEST_CLASS(Ping, command::Ping);
    
//=======================================================
    
std::string EzyHandshakeRequestParams::getClientType() {
    return "C++";
}
    
std::string EzyHandshakeRequestParams::getClientVersion() {
    return "1.0.0";
}
    
entity::EzyArray* EzyHandshakeRequestParams::serialize() {
    auto array = new entity::EzyArray();
    array->addString(mClientId);
    array->addString(mClientKey);
    array->addString(mReconnectToken);
    array->addString(getClientType());
    array->addString(getClientVersion());
    return array;
}
    
EZY_IMPLEMENT_PARAMS_REQUEST_CLASS(Handshake, command::Handshake);
    
//=======================================================

EzyLoginRequestParams::~EzyLoginRequestParams() {
}
    
entity::EzyArray* EzyLoginRequestParams::serialize() {
    auto array = new entity::EzyArray();
    array->addString(mUsername);
    array->addString(mPassword);
    array->addArray(mData);
    return array;
}

EZY_IMPLEMENT_PARAMS_REQUEST_CLASS(Login, command::Login)
    
//=======================================================

EzyAccessAppRequestParams::~EzyAccessAppRequestParams() {
}
    
entity::EzyArray* EzyAccessAppRequestParams::serialize() {
    auto array = new entity::EzyArray();
    array->addString(mAppName);
    array->addItem(mData);
    return array;
}
    
EZY_IMPLEMENT_PARAMS_REQUEST_CLASS(AccessApp, command::AppAccess)
    
}
EZY_NAMESPACE_END
