#pragma once

#include "EzySocketAdapter.h"
#include "../codec/EzyDataDecoder.h"

EZY_NAMESPACE_START
namespace socket {

class EzySocketReader : public EzySocketAdapter, public codec::EzyDataDecoderDelegate {
protected:
#ifdef USE_MESSAGE_HEADER
    std::vector<char> mByteBuffer;
    int mDataSize;
    codec::EzyDecodeState mDecodeState;
    codec::EzyMessageHeader* mMessageHeader;
    
    virtual void onRecvData();
    virtual void onUpdateDataHeader();
    virtual void onUpdateDataSize();
    virtual void onUpdateData();
#endif
    
    codec::EzyDataDecoder* mDecoder;
    virtual void recvData(const char* data, size_t size);
    virtual void onRecvMessage(entity::EzyValue* value);
public:
    EzySocketReader();
    virtual ~EzySocketReader();
    
    virtual void updateThread();
};
    
}
EZY_NAMESPACE_END
