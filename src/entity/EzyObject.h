#pragma once

#include "EzyValue.h"
#include "../codec/EzyDataEncoder.h"

EZY_NAMESPACE_START
namespace entity {

class EzyObject : public EzyValue {
protected:
	std::map<std::string, EzyValue*> data;
#ifdef EZY_DEBUG
	virtual void printToOutStream(std::ostringstream& stream, int padding);
#endif
	virtual void toValue(rapidjson::Value& value, rapidjson::Document::AllocatorType& allocator);
public:
	EzyObject();
	virtual ~EzyObject();
	virtual void writeToBuffer(codec::EzyDataEncoder* encoder);

	bool isExistKey(const std::string& key);
	int size();
	void clear();

	void addItem(const std::string& key, EzyValue* item);
	EzyValue* getItem(const std::string& key);

	bool getBool(const std::string& key, bool defaultValue = 0);
	float getFloat(const std::string& key, float defaultValue = 0);
	double getDouble(const std::string& key, double defaultValue = 0);
	int64_t getInt(const std::string& key, int64_t defaultValue = 0);
	uint64_t getUInt(const std::string& key, uint64_t defaultValue = 0);
	const std::string& getString(const std::string& key, const std::string& defaultValue = "");
	EzyObject* getObject(const std::string& key, EzyObject* defaultValue = 0);
	EzyArray* getArray(const std::string& key, EzyArray* defaultValue = 0);

	void setBool(const std::string& key, bool value);
	void setFloat(const std::string& key, float value);
	void setDouble(const std::string& key, double value);
	void setInt(const std::string& key, int64_t value);
	void setUInt(const std::string& key, uint64_t value);
	void setString(const std::string& key, const std::string& value);
	EzyObject* setObject(const std::string& key, EzyObject* value = 0);
	EzyArray* setArray(const std::string& key, EzyArray* value = 0);

	static EzyObject* create();
};

}
EZY_NAMESPACE_END
