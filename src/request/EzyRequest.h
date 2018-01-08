#pragma once

#include "../EzyMacro.h"
#include "../entity/EzyArray.h"
#include "../constant/EzyCommand.h"
#include "../io/EzyArrayDataSerializable.h"

#define EZY_DECLARE_PARAMS_REQUEST_CLASS(className) \
class Ezy##className##Request : public EzyRequest {\
public:\
    Ezy##className##Request(Ezy##className##RequestParams* params);\
    static Ezy##className##Request* create(Ezy##className##RequestParams* params);\
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
    EZY_SYNTHESIZE_READONLY(command::EzyCommand, Command)
    EZY_SYNTHESIZE_READONLY(EzyParams*, Params)
public:
    EzyRequest(command::EzyCommand cmd, EzyParams* params);
    virtual ~EzyRequest();
    static EzyRequest* create(command::EzyCommand cmd, EzyParams* params);
};

//==========================================
    
EZY_DECLARE_REQUEST_CLASS(Ping);

//==========================================
    
class EzyHandshakeRequestParams : public EzyParams {
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
    
class EzyLoginRequestParams : public EzyParams {
protected:
    EZY_SYNTHESIZE(std::string, Username)
    EZY_SYNTHESIZE(std::string, Password)
    EZY_SYNTHESIZE(entity::EzyArray*, Data)
public:
    ~EzyLoginRequestParams();
    entity::EzyArray* serialize();
};
    
EZY_DECLARE_PARAMS_REQUEST_CLASS(Login);
    
//==========================================
    
class EzyAccessAppRequestParams : public EzyParams {
protected:
    EZY_SYNTHESIZE(std::string, AppName);
    EZY_SYNTHESIZE(entity::EzyValue*, Data);
public:
    ~EzyAccessAppRequestParams();
    entity::EzyArray* serialize();
};
    
EZY_DECLARE_PARAMS_REQUEST_CLASS(AccessApp)

}

EZY_NAMESPACE_END


