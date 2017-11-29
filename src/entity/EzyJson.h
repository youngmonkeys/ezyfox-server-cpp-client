#pragma once

#include "EzyValue.h"
#include "../codec/EzyDataEncoder.h"

EZY_NAMESPACE_START
namespace entity {

class EzyJson : public EzyValue {
	std::string jsonStr;
	EzyValue* value;

	void initWithJson(const std::string& json);
	void initWithValue(EzyValue* value);
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

}
EZY_NAMESPACE_END
