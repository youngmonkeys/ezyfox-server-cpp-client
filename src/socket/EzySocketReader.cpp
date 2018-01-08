#include "EzySocketReader.h"
#include "../logger/EzyLogger.h"
#include "../entity/EzyJson.h"

EZY_NAMESPACE_START
namespace socket {

EzySocketReader::EzySocketReader(){
    mSocketPool = new EzySocketPool();
    mDecoder = new codec::EzyDataDecoder();
    mDecoder->setDelegate(this);
    mMessageHeader = new codec::EzyMessageHeader();
}

EzySocketReader::~EzySocketReader(){
    EZY_SAFE_DELETE(mDecoder)
}

void EzySocketReader::updateThread(){
#ifdef USE_MESSAGE_HEADER
    mDecodeState = codec::prepareMessage;
    mDataSize = 0;
    mByteBuffer.reserve(100 * 1024); // 100KB RAM
#endif
    EzySocketAdapter::updateThread();
}

void EzySocketReader::recvData(const char* data, size_t size){
    if (size <= 0){
        return;
    }
#ifdef USE_MESSAGE_HEADER
    mByteBuffer.insert(mByteBuffer.end(), data, data + size);
    onRecvData();
#else
    mDecoder->addData(data, size);
#endif
}


#ifdef USE_MESSAGE_HEADER
void EzySocketReader::onRecvData(){
    if (mByteBuffer.size() <= 0){
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
            onRecvData();
            break;
    }
}

void EzySocketReader::onUpdateDataHeader() {
    if(mByteBuffer.size() >= 1) {
        char headerByte;
        memcpy(&headerByte, mByteBuffer.data(), sizeof(headerByte));
        mByteBuffer.erase(mByteBuffer.begin(), mByteBuffer.begin() + 1);
        mDecodeState = codec::readMessageSize;
        onRecvData();
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
        onRecvData();
    }
}

void EzySocketReader::onUpdateData() {
    if (mByteBuffer.size() >= mDataSize){
        mDecoder->addData(mByteBuffer.data(), mDataSize);
        mByteBuffer.erase(mByteBuffer.begin(), mByteBuffer.begin() + mDataSize);
        mDecodeState = codec::prepareMessage;
        onRecvData();
    }
}
#endif

void EzySocketReader::onRecvMessage(entity::EzyValue* value){
    if (!value){
#ifdef EZY_DEBUG
        logger::log("error parse data");
#endif
        setRunning(false);
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

}
EZY_NAMESPACE_END
