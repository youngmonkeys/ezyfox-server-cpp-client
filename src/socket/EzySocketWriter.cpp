
#include "EzySocketWriter.h"
#include "../logger/EzyLogger.h"
#include "../codec/EzyMessage.h"
#include "../config/EzyClientConfig.h"

EZY_NAMESPACE_START_WITH(socket)

EzySocketWriter::EzySocketWriter(config::EzySocketConfig* config) {
    mEncoder = new codec::EzyDataEncoder(config->getEncodeReserveSize());
}

EzySocketWriter::~EzySocketWriter() {
    delete mEncoder;
}

void EzySocketWriter::toBufferData(EzySocketData* data) {
    mEncoder->clear();
    data->writeToBuffer(mEncoder);
    data->release();
    
#ifdef USE_MESSAGE_HEADER
    auto header = codec::EzyMessageHeader::create(mEncoder->getSize());
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

void EzySocketWriter::offerMessage(EzySocketData* data) {
    mSocketPool->offer(data);
}

EZY_NAMESPACE_END_WITH
