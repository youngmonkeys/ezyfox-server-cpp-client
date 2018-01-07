#include <iostream>
#include <thread>
#include <chrono>
#include "EzyClient.h"
#include "socket/EzySocketTcpClient.h"

using namespace EZY_NAMESPACE;

int main(int argc, const char * argv[]) {
    srand( static_cast<unsigned int>(time(NULL)));
    EzyClient *client = new EzyClient();
    logger::log("start client");
    client->connect("tvd12.com", 3005);
    do {
        client->processSocketEvent();
        std::this_thread::sleep_for(std::chrono::milliseconds(3));
    } while(true);
    logger::log("shutdown client");
    return 0;
}
