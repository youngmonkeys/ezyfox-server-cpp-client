//
//  main.cpp
//  ezyfox-server-cpp-client
//
//  Created by Dung Ta Van on 11/28/17.
//  Copyright © 2017 Young Monkeys. All rights reserved.
//

#include <iostream>
#include "EzyClient.h"

using namespace com::tvd12::ezyfoxserver::client;

int main(int argc, const char * argv[]) {
    // insert code here...
    auto object = new entity::EzyObject();
    object->setInt("1", 100);
    object->setString("2", "hello world");
    
    auto json = entity::EzyJson::create(object);
    
    logger::log("my json: %s", json->toString().c_str());
    
//    EZY_DELETE_0(object);
    EZY_DELETE_0(json);
    
    return 0;
}
