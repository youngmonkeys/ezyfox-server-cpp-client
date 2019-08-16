#pragma once

#include "../EzyMacro.h"
#include "../entity/EzyArray.h"

EZY_NAMESPACE_START
class EzyClient;
EZY_NAMESPACE_END

EZY_NAMESPACE_START_WITH_ONLY(entity)
class EzyUser;
class EzyZone;
class EzyApp;
EZY_NAMESPACE_END_WITH

EZY_NAMESPACE_START_WITH_ONLY(socket)
class EzyPingSchedule;
EZY_NAMESPACE_END_WITH

EZY_NAMESPACE_START_WITH_ONLY(request)
class EzyRequest;
EZY_NAMESPACE_END_WITH

EZY_NAMESPACE_START_WITH(handler)

class EzyAppDataHandlers;

class EzyDataHandler {
protected:
    EzyClient* mClient;
public:
    EzyDataHandler();
    virtual ~EzyDataHandler();
    virtual void handle(entity::EzyArray* data) = 0;
public:
    virtual void setClient(EzyClient* client);
};

//===============================================

class EzyPongHandler : public EzyDataHandler {
public:
    void handle(entity::EzyArray* data);
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
    EzyHandshakeHandler();
    virtual ~EzyHandshakeHandler();
    void handle(entity::EzyArray* data);
};

//===============================================

class EzyLoginSuccessHandler : public EzyDataHandler {
protected:
    virtual entity::EzyUser* newUser(entity::EzyArray* data);
    virtual entity::EzyZone* newZone(entity::EzyArray* data);
    virtual void handleLoginSuccess(entity::EzyArray* joinedApps,
                                    entity::EzyValue* responseData);
public:
    void handle(entity::EzyArray* data);
};

//===============================================

//===============================================

class EzyLoginErrorHandler : public EzyDataHandler {
protected:
    virtual void handleLoginError(entity::EzyArray* data);
public:
    void handle(entity::EzyArray* data);
};

//===============================================

class EzyAccessAppHandler : public EzyDataHandler {
protected:
    virtual void postHandle(entity::EzyApp* app, entity::EzyArray* data);
    virtual entity::EzyApp* newApp(entity::EzyZone* zone, entity::EzyArray* data);
public:
    void handle(entity::EzyArray* data);
};

//===============================================

class EzyAbstractAppResponseHandler : public EzyDataHandler {
public:
    virtual void handle(entity::EzyArray* data);
protected:
    virtual void handle(EzyAppDataHandlers* handlers, entity::EzyApp* app, entity::EzyArray* data) = 0;
};

class EzyAppResponseByIntHandler : public EzyAbstractAppResponseHandler {
protected:
    void handle(EzyAppDataHandlers* handlers, entity::EzyApp* app, entity::EzyArray* data);
};

class EzyAppResponseByStringHandler : public EzyAbstractAppResponseHandler {
protected:
    void handle(EzyAppDataHandlers* handlers, entity::EzyApp* app, entity::EzyArray* data);
};

//===============================================

EZY_NAMESPACE_END_WITH
