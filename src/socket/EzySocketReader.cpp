#include "EzySocketReader.h"
#include "../logger/EzyLogger.h"
#include "../entity/EzyJson.h"

EZY_NAMESPACE_START_WITH(socket)

EzySocketReader::EzySocketReader(){
    mSocketPool = new EzySocketPool();
    mMessageHeader = new codec::EzyMessageHeader();
    mDecoder = new codec::EzyDataDecoder();
    mDecoder->setDelegate(this);
}

EzySocketReader::~EzySocketReader(){
    EZY_SAFE_DELETE(mDecoder)
    EZY_SAFE_DELETE(mMessageHeader);
}

void EzySocketReader::run(){
#ifdef USE_MESSAGE_HEADER
    mDecodeState = codec::prepareMessage;
    mDataSize = 0;
    mByteBuffer.reserve(100 * 1024); // 100KB RAM
#endif
    EzySocketAdapter::run();
}

void EzySocketReader::acceptData(const char* data, size_t size){
    if (size <= 0){
        return;
    }
#ifdef USE_MESSAGE_HEADER
    mByteBuffer.insert(mByteBuffer.end(), data, data + size);
    onDataReceived();
#else
    mDecoder->addData(data, size);
#endif
}


#ifdef USE_MESSAGE_HEADER
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
    if(mByteBuffer.size() >= 1) {
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
    if (mByteBuffer.size() >= dataSizeLength){
        if(bigSize){
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
        mDecoder->addData(mByteBuffer.data(), mDataSize);
        mByteBuffer.erase(mByteBuffer.begin(), mByteBuffer.begin() + mDataSize);
        mDecodeState = codec::prepareMessage;
        onDataReceived();
    }
}
#endif

void EzySocketReader::onReceivedMessage(entity::EzyValue* value){
    if (!value){
#ifdef EZY_DEBUG
        logger::log("error parse data");
#endif
        setActive(false);
        return;
    }
    
#ifdef EZY_DEBUG
    logger::console("\n-------------------\n");
    logger::console("[RECV] ==>\n");
    value->printDebug();
    logger::console("\n-------------------\n");
#endif
    pushMessage(value);
}

EZY_NAMESPACE_END_WITH
