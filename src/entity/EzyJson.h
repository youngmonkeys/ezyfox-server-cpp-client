#pragma once

#include "EzyValue.h"
#include "../codec/EzyDataEncoder.h"

EZY_NAMESPACE_START_WITH(entity)

class EzyJson : public EzyValue {
    EzyValue* mValue;
	std::string mString;

    void initWithValue(EzyValue* value);
	void initWithString(const std::string& json);
public:
	EzyJson();
	virtual ~EzyJson();
    virtual void writeToBuffer(codec::EzyDataEncoder* encoder);
#ifdef EZY_DEBUG
	virtual void printDebug();
#endif

    EzyObject* getValue();
	const std::string& toString();

    static EzyJson* create(EzyValue* value);
	static EzyJson* create(const std::string& json);
};

EZY_NAMESPACE_END_WITH
