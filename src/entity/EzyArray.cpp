#include "EzyArray.h"
#include "EzyObject.h"
#include "EzyPrimitive.h"
#include "EzyString.h"
#include "../logger/EzyLogger.h"

EZY_NAMESPACE_START_WITH(entity)

EzyArray::EzyArray() {
	mValueType = EzyValueType::TypeArray;
}

void EzyArray::writeToBuffer(codec::EzyDataEncoder* encoder){
	encoder->writeArray((uint32_t)mData.size());
	if (mData.size() > 0) {
		for (int i = 0; i < mData.size(); i++){
			mData[i]->writeToBuffer(encoder);
		}
	}	
}

EzyArray::~EzyArray() {
	this->clear();
}

EzyArray* EzyArray::create(){
	auto value = new EzyArray();
	value->autorelease();
	return value;
}
#ifdef EZY_DEBUG
void EzyArray::printToOutStream(std::ostringstream& outStream, int padding){
	outStream << "[Array](" << mData.size() << ")" << std::endl;
	refreshLogBuffer(outStream);

	this->printPadding(outStream, padding);
	outStream << "{" << std::endl;
	for (int i = 0; i < mData.size(); i++){
		this->printPadding(outStream, padding + 1);
		mData[i]->printToOutStream(outStream, padding + 1);
		outStream << std::endl;
		refreshLogBuffer(outStream);
	}
	this->printPadding(outStream, padding);
	outStream << "}";
}
#endif
void EzyArray::toValue(rapidjson::Value& value, rapidjson::Document::AllocatorType& allocator){
	value.SetArray();
	for (int i = 0; i < mData.size(); i++){
		rapidjson::Value obj;
		mData[i]->toValue(obj, allocator);
		value.PushBack(obj, allocator);
	}
}

void EzyArray::addItem(EzyValue* item){
	mData.push_back(item);
	item->retain();
}

EzyValue* EzyArray::getItem(int index){
	return mData[index];
}

int EzyArray::size(){
	return (int)mData.size();
}

void EzyArray::clear(){
	for (int i = 0; i < mData.size(); i++){
		mData[i]->release();
	}
	mData.clear();
}

void EzyArray::addBool(bool value){
	auto item = new EzyPrimitive();
	item->setBool(value);
	this->addItem(item);
	item->release();
}

void EzyArray::addFloat(float value){
	auto item = new EzyPrimitive();
	item->setFloat(value);
	this->addItem(item);
	item->release();
}

void EzyArray::addDouble(double value){
	auto item = new EzyPrimitive();
	item->setDouble(value);
	this->addItem(item);
	item->release();
}

void EzyArray::addInt(int64_t value){
	auto item = new EzyPrimitive();
	item->setInt(value);
	this->addItem(item);
	item->release();
}

void EzyArray::addUInt(uint64_t value){
	auto item = new EzyPrimitive();
	item->setUInt(value);
	this->addItem(item);
	item->release();
}

void EzyArray::addString(const std::string& value){
	auto item = new EzyString();
	item->setString(value);
	this->addItem(item);
	item->release();
}


EzyObject* EzyArray::addObject(EzyObject* value){
	if (value){
		this->addItem(value);
	}
	else{
		value = new EzyObject();
		this->addItem(value);
		value->release();
	}
	return value;
}

EzyArray* EzyArray::addArray(EzyArray* value){
	if (value){
		this->addItem(value);
	}
	else{
		value = new EzyArray();
		this->addItem(value);
		value->release();
	}
	return value;
}

bool EzyArray::getBool(int index){
	return ((EzyPrimitive*)getItem(index))->getBool();
}

float EzyArray::getFloat(int index){
	return ((EzyPrimitive*)getItem(index))->getFloat();
}

double EzyArray::getDouble(int index){
	return ((EzyPrimitive*)getItem(index))->getDouble();
}

int64_t EzyArray::getInt(int index){
	return ((EzyPrimitive*)getItem(index))->getInt();
}

uint64_t EzyArray::getUInt(int index){
	return ((EzyPrimitive*)getItem(index))->getUInt();
}

std::string EzyArray::getString(int index){
	return ((EzyString*)getItem(index))->getString();
}

EzyObject* EzyArray::getObject(int index){
	return ((EzyObject*)getItem(index));
}

EzyArray* EzyArray::getArray(int index){
	return ((EzyArray*)getItem(index));
}

EZY_NAMESPACE_END_WITH
