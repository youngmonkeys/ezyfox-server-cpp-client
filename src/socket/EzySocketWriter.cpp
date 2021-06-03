
#include "EzySocketWriter.h"
#include "../logger/EzyLogger.h"
#include "../codec/EzyMessage.h"
#include "../codec/EzyEncryption.h"
#include "../config/EzyClientConfig.h"

EZY_NAMESPACE_START_WITH(socket)

EzySocketWriter::EzySocketWriter(config::EzySocketConfig* config) {
    mEncryptionKey.clear();
    mEncoder = new codec::EzyDataEncoder(config->getEncodeReserveSize());
}

EzySocketWriter::~EzySocketWriter() {
    delete mEncoder;
}

void EzySocketWriter::toBufferData(EzySocketData* data, bool encrypted) {
    mEncoder->clear();
    data->writeToBuffer(mEncoder);
    data->release();
    
    if(encrypted && mEncryptionKey.size() > 0) {
        int actualDataSize = 0;
        auto clearData = (char*)mEncoder->getBuffer().data();
        auto encryption = codec::EzyAES::getInstance()->encrypt(clearData,
                                                                mEncoder->getSize(),
                                                                mEncryptionKey,
                                                                actualDataSize);
        mEncoder->replaceBuffer(encryption, actualDataSize);
    }
    
#ifdef USE_MESSAGE_HEADER
    auto header = codec::EzyMessageHeader::create(mEncoder->getSize(), encrypted);
    if(header->isBigSize()) {
        uint32_t dataSize = mEncoder->getSize();
        dataSize = htonl(dataSize);
        mEncoder->insertDataSize(header, dataSize);
    }
    else {
        uint16_t dataSize = mEncoder->getSize();
        dataSize = htons(dataSize);
        mEncoder->insertDataSize(header, dataSize);
    }
    mEncoder->insertHeader(header);
#endif
}

void EzySocketWriter::offerMessage(EzySocketData* data, bool encrypted) {
    mSocketPool->offer(data, encrypted);
}

EZY_NAMESPACE_END_WITH
