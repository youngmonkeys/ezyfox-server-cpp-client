//
//  main.cpp
//  ezyfox-example
//
//  Created by Dzung on 08/08/2021.
//

#include <iostream>
#include "EzyHeaders.h"

const char* ZONE_NAME = "example";
const char* APP_NAME = "hello-world";

class HandshakeHandler : public handler::EzyHandshakeHandler {
protected:
    request::EzyRequest* getLoginRequest() {
        auto request = request::EzyLoginRequest::create();
        request->setZoneName(ZONE_NAME);
        request->setUsername("dungtv");
        request->setPassword("123456");
        return request;
    };
};

class LoginSuccessHandler : public handler::EzyLoginSuccessHandler {
protected:
    void handleLoginSuccess(entity::EzyValue* responseData) {
        auto appRequest = request::EzyAppAccessRequest::create();
        appRequest->setAppName(APP_NAME);
        mClient->send(appRequest);
    }
};

class UdpHandshakeHandler : public handler::EzyUdpHandshakeHandler {
protected:
    void onAuthenticated(entity::EzyArray *data) override {
        auto requestData = new entity::EzyObject();
        requestData->setString("who", "C/C++ Developer");
        mClient->getApp()->send("udpGreet", requestData);
    }
};

class AppAccessHandler : public handler::EzyAppAccessHandler {
protected:
    void postHandle(entity::EzyApp* app, entity::EzyArray* data) {
        auto requestData = new entity::EzyObject();
        requestData->setString("who", "C/C++ Developer");
        app->send("greet", requestData);
        mClient->udpConnect(2611);
    }
};

class GreetResponseHandler : public handler::EzyAbstractAppDataHandler<entity::EzyObject> {
protected:
    void process(entity::EzyApp* app, entity::EzyObject* data) {
        logger::log("recived message: %s", data->getString("message").c_str());
    }
};

int main(int argc, const char * argv[]) {
    auto config = config::EzyClientConfig::create();
    config->setClientName(ZONE_NAME);
    auto mSocketClient = EzyUTClient::create(config);
    EzyClients::getInstance()->addClient(mSocketClient);
    auto setup = mSocketClient->setup();
    setup->addDataHandler(constant::Handshake, new HandshakeHandler());
    setup->addDataHandler(constant::Login, new LoginSuccessHandler());
    setup->addDataHandler(constant::UdpHandshake, new UdpHandshakeHandler());
    setup->addDataHandler(constant::AppAccess, new AppAccessHandler());
    
    auto appSetup = setup->setupApp("hello-world");
    appSetup->addDataHandler("greet", new GreetResponseHandler());
    
    mSocketClient->connect("tvd12.com", 3005);
    
    socket::EzyMainEventsLoop* eventsLoop = new socket::EzyMainEventsLoop();
    eventsLoop->start();
    EZY_SAFE_DELETE(eventsLoop);
}
