#pragma once

#define RAPIDJSON_HAS_STDSTRING 1

#include <sstream>
#include "EzyMacro.h"
#include "../base/EzyRef.h"
#include "../rapidjson/rapidjson.h"
#include "../rapidjson/document.h"
#include "../codec/EzyDataEncoder.h"

EZY_NAMESPACE_START_WITH(entity)

enum EzyValueType {
	TypeNULL = 0,
	TypeBool = 1,
	TypeFloat = 2,
	TypeDouble = 3,
	TypeInt = 4,
	TypeUInt = 5,
	TypeString = 6,
	TypeDict = 7,
	TypeArray = 8,
    TypeJson = 101,
};

class EzyPrimitive;
class EzyObject;
class EzyArray;
class EzyString;

class EzyValue : public base::EzyRef {
	friend EzyPrimitive;
	friend EzyObject;
	friend EzyArray;
	friend EzyString;
protected:
#ifdef EZY_DEBUG
	virtual void refreshLogBuffer(std::ostringstream& stream);
	virtual void printToOutStream(std::ostringstream& stream, int padding);
	virtual void printPadding(std::ostringstream& stream, int padding);
#endif
	virtual void toValue(rapidjson::Value& value, rapidjson::Document::AllocatorType& allocator);
public:
	int mValueType;
public:
	EzyValue();
	virtual ~EzyValue();
    virtual void writeToBuffer(codec::EzyDataEncoder* encoder);
#ifdef EZY_DEBUG
	virtual void printDebug();
#endif

	std::string toJson();
};

EZY_NAMESPACE_END_WITH
