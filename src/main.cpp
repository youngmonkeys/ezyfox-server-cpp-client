#include <iostream>
#include <thread>
#include <chrono>
#include "EzyHeaders.h"

using namespace EZY_NAMESPACE;

class ExHandshakeHandler : public handler::EzyHandshakeHandler {
protected:
    request::EzyRequest* getLoginRequest() {
        auto request = new request::EzyLoginRequest();
        request->setZoneName("freechat");
        request->setUsername("DungTV");
        request->setPassword("123456");
        return request;
    };
};

class ExLoginSuccessHandler : public handler::EzyLoginSuccessHandler {
protected:
    void handleLoginSuccess(entity::EzyValue* responseData) {
        auto request = new request::EzyAppAccessRequest();
        request->setAppName("freechat");
        request->setData(new entity::EzyArray());
        mClient->send(request);
    }
};

int main(int argc, const char * argv[]) {
    srand( static_cast<unsigned int>(time(NULL)));
    EzyClient *client = new EzyClient();
    auto setup = client->setup();
    setup->addEventHandler(event::ConnectionSuccess, new handler::EzyConnectionSuccessHandler());
    setup->addDataHandler(constant::Handshake, new ExHandshakeHandler());
    setup->addDataHandler(constant::Login, new ExLoginSuccessHandler());
    std::cout << "start client";
    client->connect("127.0.0.1", 3005);
    do {
        client->processEvents();
        std::this_thread::sleep_for(std::chrono::milliseconds(3));
    } while(true);
//    logger::log("shutdown client");
    return 0;
}
