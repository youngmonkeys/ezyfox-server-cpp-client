#pragma once

#include "EzyValue.h"
#include "../codec/EzyDataEncoder.h"

EZY_NAMESPACE_START_WITH(entity)

class EzyPrimitive : public EzyValue {
	union
	{
		bool boolValue;
		float floatValue;
		double doubleValue;
		int64_t i64Value;
		uint64_t ui64Value;
	} data;
#ifdef EZY_DEBUG
	virtual void printToOutStream(std::ostringstream& stream, int padding);
#endif
	virtual void toValue(rapidjson::Value& value, rapidjson::Document::AllocatorType& allocator);
public:
	EzyPrimitive();
	virtual ~EzyPrimitive();
    virtual void writeToBuffer(codec::EzyDataEncoder* encoder);

	void setBool(bool b);
	void setFloat(float f);
	void setDouble(double d);
	void setInt(int64_t i64);
	void setUInt(uint64_t ui64);

	bool getBool();
	float getFloat();
	double getDouble();
	int64_t getInt();
	uint64_t getUInt();
};

EZY_NAMESPACE_END_WITH
