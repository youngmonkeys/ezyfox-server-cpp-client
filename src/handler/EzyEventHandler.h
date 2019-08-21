#pragma one

#include "../EzyMacro.h"
#include "../event/EzyEvent.h"

EZY_NAMESPACE_START
class EzyClient;
EZY_NAMESPACE_END

EZY_NAMESPACE_START_WITH_ONLY(request)
class EzyRequest;
EZY_NAMESPACE_END_WITH

EZY_NAMESPACE_START_WITH(handler)

class EzyEventHandler {
protected:
    EzyClient* mClient;
public:
    EzyEventHandler();
    virtual ~EzyEventHandler();
    virtual void handle(event::EzyEvent* event) = 0;
public:
    void setClient(EzyClient* client);
};

template <class T>
class EzyAbstractEventHandler : public EzyEventHandler {
protected:
    virtual void process(T *event) = 0;
public:
    void handle(event::EzyEvent* event);
};

template <class T>
void EzyAbstractEventHandler<T>::handle(event::EzyEvent* event) {
    this->process((T*)event);
}

//==========================================================
class EzyConnectionSuccessHandler : public EzyAbstractEventHandler<event::EzyEvent> {
protected:
    void process(event::EzyEvent* event);
protected:
    virtual void postHandle();
    virtual std::string getClientId();
    virtual std::string getClientKey();
    virtual bool isEnableEncryption();
    virtual std::string getStoredToken();
    void sendHandshakeRequest();
    request::EzyRequest* newHandshakeRequest();
};
//==========================================================

//==========================================================
class EzyDisconnectionHandler : public EzyAbstractEventHandler<event::EzyDisconnectionEvent> {
protected:
    void process(event::EzyDisconnectionEvent* event);
protected:
    virtual void preHandle(event::EzyDisconnectionEvent* event);
    virtual bool shouldReconnect(event::EzyDisconnectionEvent* event);
    virtual void control(event::EzyDisconnectionEvent* event);
};
//==========================================================

//==========================================================
class EzyConnectionFailureHandler : public EzyAbstractEventHandler<event::EzyConnectionFailureEvent> {
protected:
    void process(event::EzyConnectionFailureEvent* event);
protected:
    virtual bool shouldReconnect(event::EzyConnectionFailureEvent* event);
    virtual void control(event::EzyConnectionFailureEvent* event);
};
//==========================================================
EZY_NAMESPACE_END_WITH
