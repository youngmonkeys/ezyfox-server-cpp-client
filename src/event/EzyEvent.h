#pragma once

#include "EzyEventType.h"
#include "../entity/EzyValue.h"
#include "../io/EzyArrayDataDeserializable.h"

#define EZY_DECLARE_EVENT_CLASS(className)\
class Ezy##className##Event : public EzyEvent {\
public:\
    static Ezy##className##Event* create();\
    EzyEventType getType();\
};

#define EZY_DECLARE_ARGS_EVENT_CLASS(className)\
class Ezy##className##Event : public EzyEvent {\
protected:\
    EZY_SYNTHESIZE_READONLY(Ezy##className##EventArgs*, Args);\
public:\
    Ezy##className##Event(Ezy##className##EventArgs* args);\
    ~Ezy##className##Event();\
    static Ezy##className##Event* create(Ezy##className##EventArgs* args);\
    EzyEventType getType();\
};

EZY_NAMESPACE_START
namespace event {

//===================================
    
class EzyEvent : public entity::EzyRef {
public:
    virtual EzyEventType getType() = 0;
};
    
//===================================
    
EZY_DECLARE_EVENT_CLASS(ConnectionSuccess)
    
//===================================
    
class EzyHandshakeEventArgs : public io::EzyArrayDataDeserializable {
protected:
    EZY_SYNTHESIZE_READONLY(std::string, ServerPublicKey)
    EZY_SYNTHESIZE_READONLY(std::string, ReconnectToken)
    EZY_SYNTHESIZE_BOOL_READONLY(Reconnect)
public:
    static EzyHandshakeEventArgs* create(entity::EzyArray* array);
    void deserialize(entity::EzyArray* array);
};

EZY_DECLARE_ARGS_EVENT_CLASS(Handshake)
    
//===================================
    
class EzyLoginEventArgs : public io::EzyArrayDataDeserializable {
protected:
    EZY_SYNTHESIZE_READONLY(int64_t, UserId)
    EZY_SYNTHESIZE_READONLY(std::string, Username);
    EZY_SYNTHESIZE_READONLY(entity::EzyArray*, JoinedApp);
    EZY_SYNTHESIZE_READONLY(entity::EzyValue*, data);
public:
    static EzyLoginEventArgs* create(entity::EzyArray* array);
    void deserialize(entity::EzyArray* array);
};
    
EZY_DECLARE_ARGS_EVENT_CLASS(Login)

}
EZY_NAMESPACE_END
