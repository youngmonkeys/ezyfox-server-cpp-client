#pragma once

#include <string>
#include "../EzyMacro.h"

EZY_NAMESPACE_START
namespace socket {

class EzyKeyPair {
protected:
    EZY_SYNTHESIZE_READONLY(std::string, PrivateKey);
    EZY_SYNTHESIZE_READONLY(std::string, PublicKey);
public:
    EzyKeyPair(std::string privateKey, std::string publicKey);
};

class EzyKeyPairGentor {
public:
    virtual EzyKeyPair* generate(int keySize) = 0;
};

class EzyRsaKeyPairGentor : public EzyKeyPairGentor {
public:
    EzyKeyPair* generate(int keySize);
};
    
}

EZY_NAMESPACE_END
