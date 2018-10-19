#pragma once

#include "../EzyMacro.h"
#include "../entity/EzyValue.h"

EZY_NAMESPACE_START
class EzyClient;
EZY_NAMESPACE_END

EZY_NAMESPACE_START_WITH_ONLY(socket)
class EzyPingSchedule;
EZY_NAMESPACE_END_WITH

EZY_NAMESPACE_START_WITH_ONLY(request)
class EzyRequest;
EZY_NAMESPACE_END_WITH

EZY_NAMESPACE_START_WITH(handler)

class EzyDataHandler {
protected:
    EzyClient* mClient;
public:
    virtual ~EzyDataHandler();
    virtual void handle(entity::EzyArray* data) = 0;
public:
    virtual void setClient(EzyClient* client);
};

//===============================================

class EzyHandshakeHandler : public EzyDataHandler {
protected:
    socket::EzyPingSchedule* mPingSchedule;
protected:
    void handleLogin(entity::EzyArray* data);
protected:
    virtual void setClient(EzyClient* client);
    virtual void postHandle(entity::EzyArray* data);
    virtual request::EzyRequest* getLoginRequest() = 0;
public:
    void handle(entity::EzyArray* data);
};

//===============================================

EZY_NAMESPACE_END_WITH