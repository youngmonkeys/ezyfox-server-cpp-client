#ifndef EzyValue_h
#define EzyValue_h

#define RAPIDJSON_HAS_STDSTRING 1

#include <sstream>
#include "../base/EzyRef.h"
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
    TypeByteArray = 9,
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
    int mValueType;
protected:
#ifdef EZY_DEBUG
	virtual void printToOutStream(std::ostringstream& stream, int padding);
	virtual void printPadding(std::ostringstream& stream, int padding);
#endif
public:
	EzyValue();
	virtual ~EzyValue();
    virtual void writeToBuffer(codec::EzyDataEncoder* encoder);
    virtual void refreshLogBuffer(std::ostringstream& stream);
#ifdef EZY_DEBUG
	virtual void printDebug();
#endif
    int getType();
public:
	virtual std::string toString() const = 0;
};

EZY_NAMESPACE_END_WITH

#endif /* EzyValue_h */
