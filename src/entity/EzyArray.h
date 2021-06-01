#ifndef EzyArray_h
#define EzyArray_h

#include "EzyValue.h"
#include "../codec/EzyDataEncoder.h"

EZY_NAMESPACE_START_WITH(entity)

class EzyArray : public EzyValue {
protected:
	std::vector<EzyValue*> mData;
#ifdef EZY_DEBUG
	virtual void printToOutStream(std::ostringstream& stream, int padding) override;
#endif
public:
	EzyArray();
	virtual ~EzyArray();

    virtual void writeToBuffer(codec::EzyDataEncoder* encoder) override;

	int size();
	void clear();

	void addItem(EzyValue* item);
	EzyValue* getItem(int index);
    EzyValue* getItem(int index, EzyValue* defValue);

	void addBool(bool value);
	void addFloat(float value);
	void addDouble(double value);
	void addInt(int64_t value);
	void addUInt(uint64_t value);
	void addString(const std::string& value);
    void addByteArray(const std::string& value);
	EzyObject* addObject(EzyObject* value = 0);
	EzyArray* addArray(EzyArray* value = 0);

	bool getBool(int index);
	float getFloat(int index);
	double getDouble(int index);
	int64_t getInt(int index);
	uint64_t getUInt(int index);
	std::string getString(int index);
	EzyObject* getObject(int index);
	EzyArray* getArray(int index);
    EzyArray* getArray(int index, EzyArray* defValue);

	static EzyArray* create();
public:
    std::string toString() const override;
};

EZY_NAMESPACE_END_WITH

#endif /* EzyArray_h */
