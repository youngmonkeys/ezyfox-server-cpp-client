#pragma once

#include "EzySocketAdapter.h"
#include "../codec/EzyDataDecoder.h"

EZY_NAMESPACE_START_WITH(socket)

class EzySocketReader : public EzySocketAdapter, public codec::EzyDataDecoderDelegate {
protected:
#ifdef USE_MESSAGE_HEADER
    std::vector<char> mByteBuffer;
    int mDataSize;
    codec::EzyDecodeState mDecodeState;
    codec::EzyMessageHeader* mMessageHeader;
    
    virtual void onDataReceived();
    virtual void onUpdateDataHeader();
    virtual void onUpdateDataSize();
    virtual void onUpdateData();
#endif
    
    codec::EzyDataDecoder* mDecoder;
    virtual void acceptData(const char* data, size_t size);
    virtual void onReceivedMessage(entity::EzyValue* value);
public:
    EzySocketReader();
    virtual ~EzySocketReader();
    virtual void run();
};

EZY_NAMESPACE_END_WITH
