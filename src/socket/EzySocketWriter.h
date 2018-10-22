#pragma once

#include "EzySocketAdapter.h"

EZY_NAMESPACE_START_WITH(socket)

class EzySocketWriter : public EzySocketAdapter {
protected:
    codec::EzyDataEncoder* mEncoder;
protected:
    void toBufferData(EzySocketData* data);
public:
    EzySocketWriter();
    virtual ~EzySocketWriter();
};

EZY_NAMESPACE_END_WITH
