#include "EzyObject.h"
#include "EzyArray.h"
#include "EzyPrimitive.h"
#include "EzyString.h"

EZY_NAMESPACE_START
namespace entity {

EzyObject::EzyObject() {
	mValueType = EzyValueType::TypeDict;
}

EzyObject::~EzyObject() {
	this->clear();
}

EzyObject* EzyObject::create(){
	auto value = new EzyObject();
	value->autorelease();
	return value;
}

void EzyObject::writeToBuffer(codec::EzyDataEncoder* encoder){
	encoder->writeMap((uint32_t)mData.size());
	if (mData.size() > 0){
		for (auto it = mData.begin(); it != mData.end(); it++){
			encoder->writeString(it->first);
			it->second->writeToBuffer(encoder);
		}
	}	
}

#ifdef EZY_DEBUG
void EzyObject::printToOutStream(std::ostringstream& outStream, int padding){
	outStream << "[Object](" << mData.size() << ")" << std::endl;
	refreshLogBuffer(outStream);

	this->printPadding(outStream, padding);
	outStream << "{" << std::endl;
	for (auto it = mData.begin(); it != mData.end(); it++){
		this->printPadding(outStream, padding + 1);
		outStream << it->first << ":";
		it->second->printToOutStream(outStream, padding + 1);
		outStream << std::endl;
		refreshLogBuffer(outStream);
	}
	this->printPadding(outStream, padding);
	outStream << "}" ;
}
#endif

void EzyObject::toValue(rapidjson::Value& value, rapidjson::Document::AllocatorType& allocator){
	value.SetObject();
	for (auto it = mData.begin(); it != mData.end(); it++){
		rapidjson::Value key(it->first, allocator);

		rapidjson::Value obj;
		it->second->toValue(obj, allocator);

		value.AddMember(key, obj, allocator);
	}
}

void EzyObject::addItem(const std::string& key, EzyValue* item){
	auto it = mData.find(key);
	if (it != mData.end()){
		it->second->release();
		mData.erase(it);
	}

	mData.insert(std::make_pair(key, item));
	item->retain();
}

EzyValue* EzyObject::getItem(const std::string& key){
	auto it = mData.find(key);
	if (it != mData.end()){
		return it->second;
	}
	return 0;
}

bool EzyObject::isExistKey(const std::string& key){
	auto it = mData.find(key);
	if (it != mData.end()){
		return true;
	}
	return false;
}

void EzyObject::clear(){
	for (auto it = mData.begin(); it != mData.end(); it++){
		it->second->release();
	}
	mData.clear();
}

int EzyObject::size(){
	return (int)mData.size();
}


bool EzyObject::getBool(const std::string& key, bool defaultValue){
	auto item = this->getItem(key);
	if (item){
		return ((EzyPrimitive*)item)->getBool();
	}
	return defaultValue;
}

float EzyObject::getFloat(const std::string& key, float defaultValue){
	auto item = this->getItem(key);
	if (item){
		return ((EzyPrimitive*)item)->getFloat();
	}
	return defaultValue;
}

double EzyObject::getDouble(const std::string& key, double defaultValue){
	auto item = this->getItem(key);
	if (item){
		return ((EzyPrimitive*)item)->getDouble();
	}
	return defaultValue;
}

int64_t EzyObject::getInt(const std::string& key, int64_t defaultValue){
	auto item = this->getItem(key);
	if (item){
		return ((EzyPrimitive*)item)->getInt();
	}
	return defaultValue;
}


uint64_t EzyObject::getUInt(const std::string& key, uint64_t defaultValue){
	auto item = this->getItem(key);
	if (item){
		return ((EzyPrimitive*)item)->getUInt();
	}
	return defaultValue;
}

const std::string& EzyObject::getString(const std::string& key, const std::string& defaultValue){
	auto item = this->getItem(key);
	if (item){
		return ((EzyString*)item)->getString();
	}
	return defaultValue;
}

EzyObject* EzyObject::getObject(const std::string& key, EzyObject* defaultValue){
	auto item = this->getItem(key);
	if (item){
		return ((EzyObject*)item);
	}
	return defaultValue;
}

EzyArray* EzyObject::getArray(const std::string& key, EzyArray* defaultValue){
	auto item = this->getItem(key);
	if (item){
		return ((EzyArray*)item);
	}
	return defaultValue;
}

void EzyObject::setBool(const std::string& key, bool value){
	auto item = new EzyPrimitive();
	item->setBool(value);
	this->addItem(key, item);
	item->release();
}

void EzyObject::setFloat(const std::string& key, float value){
	auto item = new EzyPrimitive();
	item->setFloat(value);
	this->addItem(key, item);
	item->release();
}

void EzyObject::setDouble(const std::string& key, double value){
	auto item = new EzyPrimitive();
	item->setDouble(value);
	this->addItem(key, item);
	item->release();
}

void EzyObject::setInt(const std::string& key, int64_t value){
	auto item = new EzyPrimitive();
	item->setInt(value);
	this->addItem(key, item);
	item->release();
}

void EzyObject::setUInt(const std::string& key, uint64_t value){
	auto item = new EzyPrimitive();
	item->setUInt(value);
	this->addItem(key, item);
	item->release();
}

void EzyObject::setString(const std::string& key, const std::string& value){
	auto item = new EzyString();
	item->setString(value);
	this->addItem(key, item);
	item->release();
}

EzyObject* EzyObject::setObject(const std::string& key, EzyObject* value){
	if (value){
		this->addItem(key, value);
	}
	else{
		value = new EzyObject();
		this->addItem(key, value);
		value->release();
	}
	return value;
}

EzyArray* EzyObject::setArray(const std::string& key, EzyArray* value){
	if (value){
		this->addItem(key, value);
	}
	else{
		value = new EzyArray();
		this->addItem(key, value);
		value->release();
	}
	return value;
}

}
EZY_NAMESPACE_END
