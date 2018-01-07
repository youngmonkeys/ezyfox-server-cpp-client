#pragma once

#include "../EzyMacro.h"
#include "../entity/EzyArray.h"
#include "../constant/EzyCommand.h"
#include "../io/EzyArrayDataSerializable.h"

#define EZY_DECLARE_PARAMS_REQUEST_CLASS(className) \
class Ezy##className##Request : public EzyRequest {\
public:\
    Ezy##className##Request(Ezy##className##Params* params);\
static Ezy##className##Request* create(Ezy##className##Params* params);\
};

#define EZY_DECLARE_REQUEST_CLASS(className) \
class Ezy##className##Request : public EzyRequest {\
public:\
    Ezy##className##Request();\
    static Ezy##className##Request* create();\
};

EZY_NAMESPACE_START
namespace request {

class EzyParams : public io::EzyArrayDataSerializable {
public:
    virtual ~EzyParams();
    virtual entity::EzyArray* serialize();
};

class EzyRequest : public entity::EzyRef {
protected:
    EZY_SYNTHESIZE_READONLY(constant::EzyCommand, Command)
    EZY_SYNTHESIZE_READONLY(EzyParams*, Params)
public:
    EzyRequest(constant::EzyCommand cmd, EzyParams* params);
    virtual ~EzyRequest();
    static EzyRequest* create(constant::EzyCommand cmd, EzyParams* params);
};

//==========================================
    
EZY_DECLARE_REQUEST_CLASS(Ping);

//==========================================
    
class EzyHandshakeParams : public EzyParams {
protected:
    EZY_SYNTHESIZE(std::string, ClientId)
    EZY_SYNTHESIZE(std::string, ClientKey)
    EZY_SYNTHESIZE(std::string, ReconnectToken)
public:
    std::string getClientType();
    std::string getClientVersion();
    entity::EzyArray* serialize();
};

EZY_DECLARE_PARAMS_REQUEST_CLASS(Handshake)
    
//==========================================
    
class EzyLoginParams : public EzyParams {
protected:
    EZY_SYNTHESIZE(std::string, Username)
    EZY_SYNTHESIZE(std::string, Password)
    EZY_SYNTHESIZE(entity::EzyArray*, Data)
public:
    entity::EzyArray* serialize();
};
    
EZY_DECLARE_PARAMS_REQUEST_CLASS(Login);

}

EZY_NAMESPACE_END


