# ezyfox-server-cpp-client <img src="https://github.com/youngmonkeys/ezyfox-server/blob/master/logo.png" width="48" height="48" />
c++ client for ezyfox server

# Synopsis

c++ client for ezyfox server

# Code Example

**1. Import**

```cpp
#include "EzyHeaders.h"
```

```cpp
using namespace EZY_NAMESPACE;
```

**2. Create a TCP Client**

```cpp
auto config = config::EzyClientConfig::create();
config->setClientName("first");
config->setZoneName("example");
auto client = EzyClients::getInstance()->newDefaultClient(config);
```

**3. Setup client**

```cpp
auto setup = client->setup();
setup->addEventHandler(event::ConnectionSuccess, new handler::EzyConnectionSuccessHandler());
setup->addEventHandler(event::ConnectionFailure, new handler::EzyConnectionFailureHandler());
setup->addDataHandler(constant::Handshake, new ExHandshakeHandler());
setup->addDataHandler(constant::Login, new ExLoginSuccessHandler());
setup->addDataHandler(constant::AppAccess, new ExAppAccessHandler());
```

**4. Setup app**

```cpp
auto appSetup = setup->setupApp("hello-world");
appSetup->addDataHandler("broadcastMessage", new MessageResponseHandler());
```

**5. Connect to server**

```cpp
client->connect("localhost", 3005);
```

**6. Handle socket's events on main thread**

```cpp
auto mainEventsLoop = new socket::EzyMainEventsLoop();
mainEventsLoop->start();
```

**7. Custom event handler**

```cpp
class ExHandshakeHandler : public handler::EzyHandshakeHandler {
protected:
    request::EzyRequest* getLoginRequest() {
        auto request = request::EzyLoginRequest::create();
        request->setZoneName("example");
        request->setUsername("yourname");
        request->setPassword("123456");
        return request;
    };
};

class ExLoginSuccessHandler : public handler::EzyLoginSuccessHandler {
protected:
    void handleLoginSuccess(entity::EzyArray* joinedApps, entity::EzyValue* responseData) {
        auto request = request::EzyAppAccessRequest::create();
        request->setAppName("hello-world");
        request->setData(new entity::EzyArray());
        mClient->send(request);
    }
};

class ExAppAccessHandler : public handler::EzyAccessAppHandler {
protected:
    void postHandle(entity::EzyApp* app, entity::EzyArray* data) {
        auto obj = new entity::EzyObject();
        obj->setString("message", "Hi EzyFox, I'm from C++ client");
        app->send("broadcastMessage", obj);
    }
};
```
**8. Custom app's data handler**

```cpp
class MessageResponseHandler : public handler::EzyAbstractAppDataHandler<entity::EzyValue> {
protected:
    void process(entity::EzyApp* app, entity::EzyValue* data) {
        logger::log("recived message: ");
#ifdef EZY_DEBUG
        data->printDebug();
#endif
    }
};
```
