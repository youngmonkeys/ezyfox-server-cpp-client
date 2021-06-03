//
//  EzyEncryption.cpp
//  ezyfox-client
//
//  Created by Dzung on 31/05/2021.
//

#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/x509.h>
#include <openssl/err.h>
#include <openssl/aes.h>
#include <stdio.h>
#include <string.h>
#include "EzyEncryption.h"

#define RSA_SIZE 2048
#define IV_SIZE 16
#define BLOCK_SIZE 16

static const std::string beginPublicKey = "-----BEGIN PUBLIC KEY-----";
static const std::string endPublicKey = "-----BEGIN PUBLIC KEY-----";

EZY_NAMESPACE_START_WITH(codec)

EzyKeyPair::EzyKeyPair(std::string publicKey,
                       std::string encodedPrivateKey) {
    this->mPublicKey = publicKey;
    this->mEncodedPrivateKey = encodedPrivateKey;
}

EzyKeyPair::~EzyKeyPair() {
}

EzyKeyPair* EzyKeyPair::create(std::string publicKey,
                               std::string encodedPrivateKey) {
    auto pRet = new EzyKeyPair(publicKey, encodedPrivateKey);
    pRet->autorelease();
    return pRet;
}

// =======================================
EzyRSA::EzyRSA() {
}

EzyKeyPair* EzyRSA::generateKeyPair() {
    RSA* keyPair = RSA_new();
    BIGNUM* publicKeyExponent = BN_new();
    BN_set_word(publicKeyExponent, RSA_F4);
    RSA_generate_key_ex(keyPair, RSA_SIZE, publicKeyExponent, NULL);
    
    BIO *privateKeyBIO = BIO_new(BIO_s_mem());
    BIO *publicKeyBIO = BIO_new(BIO_s_mem());
    
    PEM_write_bio_RSAPrivateKey(privateKeyBIO, keyPair, NULL, NULL, 0, NULL, NULL);
    PEM_write_bio_RSA_PUBKEY(publicKeyBIO, keyPair);
    
    int publicKeyLength = BIO_pending(publicKeyBIO);
    int rsaSize = BIO_pending(privateKeyBIO);
    char *encodedPublicKey = (char*)malloc(publicKeyLength + 1);
    char *encodedPrivateKey = (char*)malloc(rsaSize + 1);
    
    BIO_read(publicKeyBIO, encodedPublicKey, publicKeyLength);
    BIO_read(privateKeyBIO, encodedPrivateKey, rsaSize);

    encodedPrivateKey[rsaSize] = '\0';
    encodedPublicKey[publicKeyLength] = '\0';
    
#ifdef EZY_DEBUG
    printf("\npulic key:\n%s\n===========\nprivate key:\n%s\n", encodedPublicKey, encodedPrivateKey);
#endif
    std::string publicKey = decodePublicKey(encodedPublicKey, publicKeyLength);
    EzyKeyPair* answer = EzyKeyPair::create(publicKey,
                                            std::string(encodedPrivateKey, rsaSize));
    RSA_free(keyPair);
    BN_free(publicKeyExponent);
    BIO_free_all(publicKeyBIO);
    BIO_free_all(privateKeyBIO);
    EZY_SAFE_FREE(encodedPublicKey);
    return answer;
}

std::string EzyRSA::decrypt(const char* message, int size, std::string privateKey) {
    RSA* rsa = RSA_new();
    BIO* keybio = BIO_new_mem_buf((unsigned char*)privateKey.c_str(), -1);
    rsa = PEM_read_bio_RSAPrivateKey(keybio, &rsa, NULL, NULL);
    if (rsa == nullptr) {
            return 0;
    }
    int rsaSize = RSA_size(rsa);
    int totalSize = rsaSize + 1;
    char* decryption = new char[totalSize];
    memset(decryption, 0, totalSize);
    char* encryption = new char[rsaSize];
    int position = 0;
    bool hasError = false;
    int left = size;
    int flagLength = 0;
    int textSize = 0;
    char *error = (char*)malloc(128);
    std::string decryptText;
    while (position < size) {
        flagLength = left >= rsaSize ? rsaSize : left;
        left -= flagLength;
        memset(encryption, 0, rsaSize);
        memcpy(encryption, (message + position), flagLength);
        memset(decryption, 0, totalSize);
        int decryptedSize = RSA_private_decrypt(flagLength,
                                  (const unsigned char*)encryption,
                                  (unsigned char*)decryption,
                                  rsa,
                                  RSA_PKCS1_PADDING);
        if (decryptedSize >= 0) {
            decryptText.append(decryption, decryptedSize);
            textSize += decryptedSize;
            position += rsaSize;
        }
        else {
            ERR_load_crypto_strings();
            ERR_error_string(ERR_get_error(), error);
            fprintf(stderr, "error decrypting message: %s\n", error);
            hasError = true;
            break;
        }
    }
    
    EZY_SAFE_FREE(decryption);
    EZY_SAFE_FREE(encryption);
    EZY_SAFE_FREE(error);
    BIO_free_all(keybio);
    RSA_free(rsa);
    return hasError ? 0 : decryptText;
}

std::string EzyRSA::decodePublicKey(const char *publicKey, int size) {
    std::string copy(publicKey);
    copy.erase(copy.begin(), copy.begin() + beginPublicKey.size());
    copy.erase(copy.end() - endPublicKey.size(), copy.end());
    copy.erase(std::remove(copy.begin(), copy.end(), '\n'), copy.end());
    return copy;
}

//====================================================
EzyAES::EzyAES() {
}

char* EzyAES::decrypt(const char *message, int size, std::string key, int& outputSize) {
    AES_KEY aes;
    int decryptedKeyResult = AES_set_decrypt_key((unsigned char*)key.c_str(),
                                                 (int)key.length() * 8,
                                                 &aes);
    if (decryptedKeyResult < 0) {
        ERR_load_crypto_strings();
        char *error = (char*)malloc(128);
        ERR_error_string(ERR_get_error(), error);
        fprintf(stderr, "error decrypting message: %s\n", error);
        EZY_SAFE_FREE(error);
        return 0;
    }
    unsigned char* iv = (unsigned char*)malloc(IV_SIZE);
    memcpy(iv, message, IV_SIZE);
    int contentSize = size - IV_SIZE;
    unsigned char* content = ((unsigned char*)malloc(contentSize));
    memcpy(content, message + IV_SIZE, contentSize);
    unsigned char* output = (unsigned char*)malloc(contentSize);
    memset(output, 0, contentSize);
    AES_cbc_encrypt(content, output, contentSize, &aes, iv, AES_DECRYPT);
    outputSize = unpad(output, 0, contentSize);
    EZY_SAFE_FREE(iv);
    EZY_SAFE_FREE(content);
    return (char*)output;
}

int EzyAES::unpad(unsigned char* input, int offset, int size) {
    int idx = offset + size;
    char lastByte = input[idx - 1];
    int padValue = (int)lastByte & 0x0ff;
    if ((padValue < 0x01) || (padValue > BLOCK_SIZE)) {
        return -1;
    }
    int start = idx - padValue;
    if (start < offset) {
        return -1;
    }
    for (int i = start; i < idx; i++) {
        if (input[i] != lastByte) {
            return -1;
        }
    }
    return start;
}


EZY_NAMESPACE_END_WITH
