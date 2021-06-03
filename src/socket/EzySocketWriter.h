#ifndef EzySocketWriter_h
#define EzySocketWriter_h

#include "EzySocketAdapter.h"

EZY_NAMESPACE_START_WITH_ONLY(config)
class EzySocketConfig;
EZY_NAMESPACE_END_WITH

EZY_NAMESPACE_START_WITH(socket)

class EzySocketWriter : public EzySocketAdapter {
protected:
    codec::EzyDataEncoder* mEncoder;
protected:
    EZY_SYNTHESIZE_WRITEONLY(std::string, EncryptionKey);
protected:
    void toBufferData(EzySocketData* data, bool encrypted);
public:
    EzySocketWriter(config::EzySocketConfig* config);
    virtual ~EzySocketWriter();
    virtual void offerMessage(EzySocketData* data, bool encrypted);
};

EZY_NAMESPACE_END_WITH

#endif /* EzySocketWriter_h */
