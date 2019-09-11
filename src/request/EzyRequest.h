#ifndef EzyRequest_h
#define EzyRequest_h

#include <string>
#include "../EzyMacro.h"
#include "../entity/EzyArray.h"
#include "../constant/EzyCommand.h"

#define EZY_DECLARE_REQUEST_CLASS(className) \
class Ezy##className##Request : public EzyRequest {\
public:\
    Ezy##className##Request();\
    static Ezy##className##Request* create();\
    constant::EzyCommand getCommand();\
};

EZY_NAMESPACE_START_WITH(request)

class EzyRequest : public base::EzyRef {
public:
    virtual entity::EzyArray* serialize();
    virtual constant::EzyCommand getCommand() = 0;
};

EZY_DECLARE_REQUEST_CLASS(Ping);

//==========================================
    
class EzyHandshakeRequest : public EzyRequest {
protected:
    EZY_SYNTHESIZE(std::string, ClientId)
    EZY_SYNTHESIZE(std::string, ClientKey)
    EZY_SYNTHESIZE(std::string, ClientType)
    EZY_SYNTHESIZE(std::string, ClientVersion)
    EZY_SYNTHESIZE_BOOL(EnableEncryption)
    EZY_SYNTHESIZE(std::string, Token)
public:
    EzyHandshakeRequest();
    static EzyHandshakeRequest* create();
    entity::EzyArray* serialize();
    constant::EzyCommand getCommand();
};
    
//==========================================
    
class EzyLoginRequest : public EzyRequest {
protected:
    EZY_SYNTHESIZE(std::string, ZoneName)
    EZY_SYNTHESIZE(std::string, Username)
    EZY_SYNTHESIZE(std::string, Password)
    EZY_SYNTHESIZE(entity::EzyArray*, Data)
public:
    EzyLoginRequest();
    static EzyLoginRequest* create();
    entity::EzyArray* serialize();
    constant::EzyCommand getCommand();
};
    
//==========================================
    
class EzyAppAccessRequest : public EzyRequest {
protected:
    EZY_SYNTHESIZE(std::string, AppName);
    EZY_SYNTHESIZE(entity::EzyValue*, Data);
public:
    EzyAppAccessRequest();
    static EzyAppAccessRequest* create();
    entity::EzyArray* serialize();
    constant::EzyCommand getCommand();
};
    
//==========================================

class EzyAppRequestRequest : public EzyRequest {
protected:
    EZY_SYNTHESIZE(int, AppId);
    EZY_SYNTHESIZE(entity::EzyValue*, Data);
public:
    EzyAppRequestRequest();
    static EzyAppRequestRequest* create();
    entity::EzyArray* serialize();
    constant::EzyCommand getCommand();
};
    
//==========================================
    
class EzyPluginRequestRequest : public EzyRequest {
protected:
    EZY_SYNTHESIZE(int, PluginId);
    EZY_SYNTHESIZE(entity::EzyValue*, Data);
public:
    EzyPluginRequestRequest();
    static EzyPluginRequestRequest* create();
    entity::EzyArray* serialize();
    constant::EzyCommand getCommand();
};

EZY_NAMESPACE_END_WITH


#endif /* EzyRequest_h */
