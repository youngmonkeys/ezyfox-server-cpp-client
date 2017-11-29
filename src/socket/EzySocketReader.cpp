#include "EzySocketReader.h"
#include "../logger/EzyLogger.h"
#include "../entity/EzyJson.h"

EZY_NAMESPACE_START
namespace socket {

EzySocketReader::EzySocketReader(){
    mSocketPool = new EzySocketPool();
    mDecoder = new codec::EzyDataDecoder();
    mDecoder->setDelegate(this);
}

EzySocketReader::~EzySocketReader(){
    EZY_DELETE_0(mDecoder)
}

void EzySocketReader::updateThread(){
#ifdef USE_MESSAGE_HEADER
    mRecvHeader = true;
    mDataSize = 0;
    mByteBuffer.reserve(100 * 1024); // 100KB RAM
#endif
    EzySocketAdapter::updateThread();
}

void EzySocketReader::recvData(const char* data, int size){
    if (size <= 0){
        return;
    }
#ifdef USE_MESSAGE_HEADER
    mByteBuffer.insert(mByteBuffer.end(), data, data + size);
    this->onRecvData();
#else
    mDecoder->addData(data, size);
#endif
}


#ifdef USE_MESSAGE_HEADER
void EzySocketReader::onRecvData(){
    if (mByteBuffer.size() <= 0){
        return;
    }
    if (mRecvHeader){
        onUpdateDataHeader();
    }
    else{
        onUpdateData();
    }
}

void EzySocketReader::onUpdateDataHeader(){
    if (mByteBuffer.size() >= 4){
        memcpy(&mDataSize, mByteBuffer.data(), sizeof(mDataSize));
        mDataSize = ntohl(mDataSize);
        
        mRecvHeader = false;
        mByteBuffer.erase(mByteBuffer.begin(), mByteBuffer.begin() + 4);
        this->onRecvData();
    }
}

void EzySocketReader::onUpdateData(){
    if (mByteBuffer.size() >= mDataSize){
        mDecoder->addData(mByteBuffer.data(), mDataSize);
        mRecvHeader = true;
        mByteBuffer.erase(mByteBuffer.begin(), mByteBuffer.begin() + mDataSize);
        this->onRecvData();
    }
}
#endif

void EzySocketReader::onRecvMessage(entity::EzyValue* value){
    if (!value){
#ifdef EZY_DEBUG
        logger::log("error parse data");
#endif
        this->setRunning(false);
        return;
    }
    
#ifdef EZY_DEBUG
    logger::console("\n-------------------\n");
    logger::console("[RECV] ==>\n");
    value->printDebug();
    logger::console("\n-------------------\n");
#endif
    this->pushMessage(entity::EzyJson::create(value));
}

}
EZY_NAMESPACE_END
