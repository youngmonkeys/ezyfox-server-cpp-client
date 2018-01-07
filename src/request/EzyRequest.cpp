#include "EzyRequest.h"

#define EZY_IMPLEMENT_PARAMS_REQUEST_CLASS(className, command) \
Ezy##className##Request::Ezy##className##Request(Ezy##className##Params* params) :\
EzyRequest(command, params) {\
}\
Ezy##className##Request* Ezy##className##Request::create(Ezy##className##Params* params) {\
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
    
//===========================
EzyParams::~EzyParams() {
}

entity::EzyArray* EzyParams::serialize() {
    return new entity::EzyArray();
}
    
//===========================
    
EzyRequest::EzyRequest(constant::EzyCommand cmd, EzyParams* params) {
    this->mCommand = cmd;
    this->mParams = params;
}
    
EzyRequest::~EzyRequest() {
    EZY_SAFE_DELETE(mParams);
}
    
EzyRequest* EzyRequest::create(constant::EzyCommand cmd, EzyParams* params) {
    auto pRet = new EzyRequest(cmd, params);
    pRet->autorelease();
    return pRet;
}

//===========================
    
EZY_IMPLEMENT_REQUEST_CLASS(Ping, constant::ping);
    
//============================
    
std::string EzyHandshakeParams::getClientType() {
    return "C++";
}
    
std::string EzyHandshakeParams::getClientVersion() {
    return "1.0.0";
}
    
entity::EzyArray* EzyHandshakeParams::serialize() {
    auto array = new entity::EzyArray();
    array->addString(mClientId);
    array->addString(mClientKey);
    array->addString(mReconnectToken);
    array->addString(getClientType());
    array->addString(getClientVersion());
    return array;
}
    
EZY_IMPLEMENT_PARAMS_REQUEST_CLASS(Handshake, constant::handshake);
    
//============================
entity::EzyArray* EzyLoginParams::serialize() {
    auto array = new entity::EzyArray();
    array->addString(mUsername);
    array->addString(mPassword);
    array->addItem(mData);
    return array;
}

EZY_IMPLEMENT_PARAMS_REQUEST_CLASS(Login, constant::login)
    
}
EZY_NAMESPACE_END
