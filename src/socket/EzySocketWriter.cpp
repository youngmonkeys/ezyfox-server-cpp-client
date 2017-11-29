
#include "EzySocketWriter.h"
#include "../logger/EzyLogger.h"

EZY_NAMESPACE_START
namespace socket {

EzySocketWriter::EzySocketWriter(){
    mSocketPool = new EzySocketPool();
    mEncoder = new codec::EzyDataEncoder();
}

EzySocketWriter::~EzySocketWriter(){
    delete mEncoder;
}

void EzySocketWriter::toBufferData(EzySocketData* data){
    mEncoder->clear();
    data->writeToBuffer(mEncoder);
    
#ifdef USE_MESSAGE_HEADER
    uint32_t dataSize = mEncoder->getSize();
    dataSize = htonl(dataSize);
    mEncoder->insertHeader((const char*) &dataSize, 4);
#endif
    
#ifdef EZY_DEBUG
    logger::console("\n----------------\n");
    logger::console("[SEND] <==\n");
    data->printDebug();
    logger::console("\n----------------\n");
#endif
}

}
EZY_NAMESPACE_END
