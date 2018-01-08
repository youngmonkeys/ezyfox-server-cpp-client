#include "EzySocketKeyPair.h"

#include "../cryptopp/rsa.h"
#include "../cryptopp/osrng.h"
#include "../cryptopp/base64.h"

using namespace CryptoPP;

EZY_NAMESPACE_START
namespace socket {

EzyKeyPair::EzyKeyPair(std::string privateKey, std::string publicKey) {
    this->mPrivateKey = privateKey;
    this->mPublicKey = publicKey;
}

//================= rs keypair gentor ==============
EzyKeyPair* EzyRsaKeyPairGentor::generate(int keySize) {
    AutoSeededRandomPool prng;
    RSA::PrivateKey privateKey;
    privateKey.GenerateRandomWithKeySize(prng, keySize);
    RSA::PublicKey publicKey(privateKey);
    std::string privateKeyString;
    std::string publicKeyString;
    Base64Encoder privateKeyBase64Encoder(new StringSink(privateKeyString));
    Base64Encoder publicKeyBase64Encoder(new StringSink(publicKeyString));
    privateKey.DEREncode(privateKeyBase64Encoder);
    publicKey.DEREncode(publicKeyBase64Encoder);
    privateKeyBase64Encoder.MessageEnd();
    publicKeyBase64Encoder.MessageEnd();
    auto pRet = new EzyKeyPair(privateKeyString, publicKeyString);
    return pRet;
}

}
EZY_NAMESPACE_END
