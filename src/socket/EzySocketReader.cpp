#include "EzySocketReader.h"
#include "EzySocketClient.h"
#include "../logger/EzyLogger.h"
#include "../config/EzyClientConfig.h"
#include "../codec/EzyEncryption.h"

EZY_NAMESPACE_START_WITH(socket)

EzySocketReader::EzySocketReader(config::EzySocketConfig* config) {
    mByteBuffer.clear();
    mBufferSize = config->getReadBufferSize();
    mReserveSize = config->getReadReserveSize();
    mDecoder = new codec::EzyDataDecoder(config->getDecodeReserveSize());
    mDecoder->setDelegate(this);
    mMessageHeader = new codec::EzyMessageHeader();
}

EzySocketReader::~EzySocketReader() {
    EZY_SAFE_DELETE(mDecoder)
    EZY_SAFE_DELETE(mMessageHeader);
}

void EzySocketReader::run() {
#ifdef USE_MESSAGE_HEADER
    mDecodeState = codec::prepareMessage;
    mDataSize = 0;
    mByteBuffer.reserve(mReserveSize);
#endif
    EzySocketAdapter::run();
}

void EzySocketReader::acceptData(const char* data, size_t size) {
    if (size <= 0) {
        return;
    }
#ifdef USE_MESSAGE_HEADER
    preInsertData();
    mByteBuffer.insert(mByteBuffer.end(), data, data + size);
    onDataReceived();
#else
    mDecoder->addData(data, size);
#endif
}

#ifdef USE_MESSAGE_HEADER
void EzySocketReader::preInsertData() {
}

void EzySocketReader::onDataReceived() {
    if (mByteBuffer.size() <= 0) {
        return;
    }
    switch (mDecodeState) {
        case codec::readMessageHeader:
            onUpdateDataHeader();
            break;
        case codec::readMessageSize:
            onUpdateDataSize();
            break;
        case codec::readMessageContent:
            onUpdateData();
            break;
        default:
            mDecodeState = codec::readMessageHeader;
            onDataReceived();
            break;
    }
}

void EzySocketReader::onUpdateDataHeader() {
    if(mByteBuffer.size() > 0) {
        char headerByte;
        memcpy(&headerByte, mByteBuffer.data(), sizeof(headerByte));
        mMessageHeader->parse(headerByte);
        mByteBuffer.erase(mByteBuffer.begin(), mByteBuffer.begin() + 1);
        mDecodeState = codec::readMessageSize;
        onDataReceived();
    }
}
    
void EzySocketReader::onUpdateDataSize() {
    bool bigSize = mMessageHeader->isBigSize();
    int dataSizeLength = bigSize ? 4 : 2;
    if (mByteBuffer.size() >= dataSizeLength) {
        if(bigSize) {
            uint64_t dataSize;
            memcpy(&dataSize, mByteBuffer.data(), sizeof(dataSize));
            mDataSize = ntohl(dataSize);
        }
        else {
            uint16_t dataSize;
            memcpy(&dataSize, mByteBuffer.data(), sizeof(dataSize));
            mDataSize = ntohs(dataSize);
        }
        mByteBuffer.erase(mByteBuffer.begin(), mByteBuffer.begin() + dataSizeLength);
        mDecodeState = codec::readMessageContent;
        onDataReceived();
    }
}

void EzySocketReader::onUpdateData() {
    if (mByteBuffer.size() >= mDataSize) {
        auto data = (char*)mByteBuffer.data();
        auto actualDataSize = mDataSize;
        if(mMessageHeader->isEncrypted()) {
            data = codec::EzyAES::getInstance()->decrypt(data,
                                                         mDataSize,
                                                         mDecryptionKey,
                                                         actualDataSize);
        }
        mDecoder->addData(data, actualDataSize);
        mByteBuffer.erase(mByteBuffer.begin(), mByteBuffer.begin() + mDataSize);
        mDecodeState = codec::prepareMessage;
        onDataReceived();
    }
}
#endif

void EzySocketReader::onReceivedMessage(entity::EzyValue* value) {
    if (!value) {
#ifdef EZY_DEBUG
        logger::log("error parse data");
#endif
        setActive(false);
        return;
    }
    mSocketPool->push(value);
}

void EzySocketReader::popMessages(std::vector<EzySocketData*>& buffer) {
    mSocketPool->popAll(buffer);
}

void EzySocketReader::setDecryptionKey(std::string decryptionKey) {
    mDecryptionKey = decryptionKey;
}

EZY_NAMESPACE_END_WITH
