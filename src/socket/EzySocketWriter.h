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
    void toBufferData(EzySocketData* data);
public:
    EzySocketWriter(config::EzySocketConfig* config);
    virtual ~EzySocketWriter();
    virtual void offerMessage(EzySocketData* data);
};

EZY_NAMESPACE_END_WITH

#endif /* EzySocketWriter_h */
