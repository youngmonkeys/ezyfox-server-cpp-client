#pragma once

#include "EzyRef.h"

#define MAX_SMALL_MESSAGE_SIZE 65535

EZY_NAMESPACE_START_WITH(codec)

class EzyMessageHeader : public base::EzyRef {
protected:
    EZY_SYNTHESIZE_BOOL_READONLY(BigSize) //bit 1
    EZY_SYNTHESIZE_BOOL(Encrypted) //bit 2
    EZY_SYNTHESIZE_BOOL_READONLY(Compressed) //bit 3
    EZY_SYNTHESIZE_BOOL(Text) //bit 4
public:
    EzyMessageHeader();
    EzyMessageHeader(uint32_t dataSize);
    virtual ~EzyMessageHeader();
    
    virtual char getByte();
    virtual void parse(char headerByte);
    
    static EzyMessageHeader* create();
    static EzyMessageHeader* create(uint32_t dataSize);
};

EZY_NAMESPACE_END_WITH
