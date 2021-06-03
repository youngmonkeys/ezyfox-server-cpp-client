//
//  EzyEncryption.h
//  ezyfox-client
//
//  Created by Dzung on 31/05/2021.
//

#ifndef EzyEncryption_h
#define EzyEncryption_h

#include <string>
#include "../EzyMacro.h"
#include "../base/EzyRef.h"

EZY_NAMESPACE_START_WITH(codec)

class EzyKeyPair : base::EzyRef {
protected:
    EZY_SYNTHESIZE_READONLY(std::string, PublicKey)
    EZY_SYNTHESIZE_READONLY(std::string, EncodedPrivateKey)
public:
    EzyKeyPair(std::string publicKey,
               std::string encodedPrivateKey);
    ~EzyKeyPair();
public:
    static EzyKeyPair* create(std::string publicKey,
                              std::string encodedPrivateKey);
};

class EzyRSA {
public:
    EZY_SINGLETON_GET_INSTANCE(EzyRSA)
public:
    EzyKeyPair* generateKeyPair();
    std::string decrypt(const char* message, int size, std::string privateKey);
private:
    std::string decodePublicKey(const char* publicKey, int size);
};

class EzyAES {
public:
    EZY_SINGLETON_GET_INSTANCE(EzyAES)
public:
    std::string encrypt(const char* message, int size, std::string key);
    char* decrypt(const char* message, int size, std::string key, int& outputSize);
private:
    int unpad(unsigned char* input, int offset, int size);
};

EZY_NAMESPACE_END_WITH

#endif /* EzyEncryption_h */
