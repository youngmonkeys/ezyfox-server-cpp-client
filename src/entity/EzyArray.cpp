#include "EzyArray.h"
#include "EzyObject.h"
#include "EzyPrimitive.h"
#include "EzyString.h"
#include "../logger/EzyLogger.h"

EZY_NAMESPACE_START_WITH(entity)

EzyArray::EzyArray() {
    mData.clear();
	mValueType = EzyValueType::TypeArray;
}

void EzyArray::writeToBuffer(codec::EzyDataEncoder* encoder) {
	encoder->writeArray((uint32_t)mData.size());
	if (mData.size() > 0) {
		for (int i = 0; i < mData.size(); ++i) {
			mData[i]->writeToBuffer(encoder);
		}
	}	
}

EzyArray::~EzyArray() {
	this->clear();
}

EzyArray* EzyArray::create() {
	auto value = new EzyArray();
	value->autorelease();
	return value;
}
#ifdef EZY_DEBUG
void EzyArray::printToOutStream(std::ostringstream& outStream, int padding) {
	outStream << "[Array](" << mData.size() << ")" << std::endl;
	refreshLogBuffer(outStream);

	this->printPadding(outStream, padding);
	outStream << "{" << std::endl;
	for (int i = 0; i < mData.size(); ++i) {
		this->printPadding(outStream, padding + 1);
		mData[i]->printToOutStream(outStream, padding + 1);
		outStream << std::endl;
		refreshLogBuffer(outStream);
	}
	this->printPadding(outStream, padding);
	outStream << "}";
}
#endif

void EzyArray::addItem(EzyValue* item) {
	mData.push_back(item);
}

EzyValue* EzyArray::getItem(int index) {
	auto value = mData[index];
    return value;
}

EzyValue* EzyArray::getItem(int index, EzyValue* defValue) {
    if(index >= mData.size())
        return defValue;
    auto value = mData[index];
    return value;
}

int EzyArray::size() {
	return (int)mData.size();
}

void EzyArray::clear() {
	for (int i = 0; i < mData.size(); ++i) {
		mData[i]->release();
	}
	mData.clear();
}

void EzyArray::addBool(bool value) {
	auto item = new EzyPrimitive();
	item->setBool(value);
	this->addItem(item);
}

void EzyArray::addFloat(float value) {
	auto item = new EzyPrimitive();
	item->setFloat(value);
	this->addItem(item);
}

void EzyArray::addDouble(double value) {
	auto item = new EzyPrimitive();
	item->setDouble(value);
	this->addItem(item);
}

void EzyArray::addInt(int64_t value) {
	auto item = new EzyPrimitive();
	item->setInt(value);
	this->addItem(item);
}

void EzyArray::addUInt(uint64_t value) {
	auto item = new EzyPrimitive();
	item->setUInt(value);
	this->addItem(item);
}

void EzyArray::addString(const std::string& value) {
	auto item = new EzyString();
	item->setString(value);
	this->addItem(item);
}


EzyObject* EzyArray::addObject(EzyObject* value) {
	if (value) {
		this->addItem(value);
	}
	else{
		value = new EzyObject();
		this->addItem(value);
	}
	return value;
}

EzyArray* EzyArray::addArray(EzyArray* value) {
	if (value) {
		this->addItem(value);
	}
	else{
		value = new EzyArray();
		this->addItem(value);
	}
	return value;
}

bool EzyArray::getBool(int index) {
	return ((EzyPrimitive*)getItem(index))->getBool();
}

float EzyArray::getFloat(int index) {
	return ((EzyPrimitive*)getItem(index))->getFloat();
}

double EzyArray::getDouble(int index) {
	return ((EzyPrimitive*)getItem(index))->getDouble();
}

int64_t EzyArray::getInt(int index) {
	return ((EzyPrimitive*)getItem(index))->getInt();
}

uint64_t EzyArray::getUInt(int index) {
	return ((EzyPrimitive*)getItem(index))->getUInt();
}

std::string EzyArray::getString(int index) {
	return ((EzyString*)getItem(index))->getString();
}

EzyObject* EzyArray::getObject(int index) {
	return ((EzyObject*)getItem(index));
}

EzyArray* EzyArray::getArray(int index) {
	return ((EzyArray*)getItem(index));
}

EzyArray* EzyArray::getArray(int index, EzyArray* defValue) {
    if(index >= mData.size())
        return defValue;
    auto answer = ((EzyArray*)getItem(index));
    return answer;
}

std::string EzyArray::toString() const {
    auto builder = std::string("[");
    for (int i = 0; i < mData.size(); ++i) {
        builder.append(mData[i]->toString());
        if(i < mData.size() - 1) {
            builder.append(",");
        }
    }
    return builder.append("]");
}

EZY_NAMESPACE_END_WITH
