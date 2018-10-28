
#include "EzySocketWriter.h"
#include "../logger/EzyLogger.h"
#include "../codec/EzyMessage.h"

EZY_NAMESPACE_START_WITH(socket)

EzySocketWriter::EzySocketWriter() {
    mSocketPool = new EzySocketPool();
    mEncoder = new codec::EzyDataEncoder();
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

EZY_NAMESPACE_END_WITH
