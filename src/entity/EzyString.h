#ifndef EzyString_h
#define EzyString_h

#include "EzyValue.h"
#include "../EzyMacro.h"

EZY_NAMESPACE_START_WITH(entity)

class EzyString : public EzyValue {
protected:
    std::string mData;
#ifdef EZY_DEBUG
    virtual void printToOutStream(std::ostringstream& stream, int padding);
#endif
    
public:
    EzyString();
    virtual ~EzyString();
    virtual void writeToBuffer(codec::EzyDataEncoder* writer) override;
    
    void setString(const std::string& str);
    void setData(const char* data, int size);
    const std::string& getString();
public:
    std::string toString() const override;
};

EZY_NAMESPACE_END_WITH

#endif /* EzyString_h */
