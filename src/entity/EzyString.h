#pragma once

#include "EzyValue.h"
#include "../EzyMacro.h"

EZY_NAMESPACE_START_WITH(entity)

class EzyString : public EzyValue {
protected:
    std::string mData;
#ifdef EZY_DEBUG
    virtual void printToOutStream(std::ostringstream& stream, int padding);
#endif
    virtual void toValue(rapidjson::Value& value, rapidjson::Document::AllocatorType& allocator);
    
public:
    EzyString();
    virtual ~EzyString();
    virtual void writeToBuffer(codec::EzyDataEncoder* writer);
    
    void setString(const std::string& str);
    void setData(const char* data, int size);
    const std::string& getString();
};

EZY_NAMESPACE_END_WITH

