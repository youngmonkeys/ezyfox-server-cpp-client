#ifndef EzyByteArray_h
#define EzyByteArray_h

#include "EzyValue.h"
#include "../EzyMacro.h"

EZY_NAMESPACE_START_WITH(entity)

class EzyByteArray : public EzyValue {
protected:
    std::string mData;
#ifdef EZY_DEBUG
    virtual void printToOutStream(std::ostringstream& stream, int padding) override;
#endif
    
public:
    EzyByteArray();
    virtual ~EzyByteArray();
    virtual void writeToBuffer(codec::EzyDataEncoder* writer) override;
    
    void setData(std::string data);
    void setData(const char* data, int size);
    const std::string getData();
public:
    std::string toString() const override;
};

EZY_NAMESPACE_END_WITH

#endif /* EzyByteArray_h */
