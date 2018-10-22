#include <iostream>
#include <thread>
#include <chrono>
#include "EzyHeaders.h"

using namespace EZY_NAMESPACE;

class ExHandshakeHandler : public handler::EzyHandshakeHandler {
protected:
    request::EzyRequest* getLoginRequest() {
        auto request = request::EzyLoginRequest::create();
        request->setZoneName("freechat");
        request->setUsername("dungtv");
        request->setPassword("123456");
        return request;
    };
};

class ExLoginSuccessHandler : public handler::EzyLoginSuccessHandler {
protected:
    void handleLoginSuccess(entity::EzyValue* responseData) {
        auto request = request::EzyAppAccessRequest::create();
        request->setAppName("freechat");
        request->setData(entity::EzyArray::create());
        mClient->send(request);
    }
};

class ExAppAccessHandler : public handler::EzyAccessAppHandler {
protected:
    void postHandle(entity::EzyApp* app, entity::EzyArray* data) {
        auto obj = entity::EzyObject::create();
        obj->setInt("skip", 0);
        obj->setInt("limit", 100);
        app->send("5", obj);
    }
};

class ExFirstAppResponseHandler : public handler::EzyAbstractAppDataHandler<entity::EzyArray> {
protected:
    void process(entity::EzyApp* app, entity::EzyArray* data) {
    }
};

int main(int argc, const char * argv[]) {
    srand( static_cast<unsigned int>(time(NULL)));
    auto config = config::EzyClientConfig::create();
    auto client = new EzyClient(config);
    auto setup = client->setup();
    setup->addEventHandler(event::ConnectionSuccess, new handler::EzyConnectionSuccessHandler());
    setup->addDataHandler(constant::Handshake, new ExHandshakeHandler());
    setup->addDataHandler(constant::Login, new ExLoginSuccessHandler());
    setup->addDataHandler(constant::AppAccess, new ExAppAccessHandler());
    auto appSetup = setup->setupApp("freechat");
    appSetup->addDataHandler("5", new ExFirstAppResponseHandler());
    logger::log("start client");
    client->connect("127.0.0.1", 3005);
    do {
        client->processEvents();
        std::this_thread::sleep_for(std::chrono::milliseconds(3));
    } while(true);
//    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
//    EZY_SAFE_DELETE(client);
//    while (1);
//    logger::log("shutdown client");
    return 0;
}
