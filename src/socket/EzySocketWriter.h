#pragma once

#include "EzySocketAdapter.h"

EZY_NAMESPACE_START
namespace socket {

class EzySocketWriter : public EzySocketAdapter {
protected:
    codec::EzyDataEncoder* mEncoder;
    void toBufferData(EzySocketData* data);
public:
    EzySocketWriter();
    virtual ~EzySocketWriter();
};
    
}
EZY_NAMESPACE_END
