#include <iomanip>
#include "EzyJson.h"
#include "EzyObject.h"
#include "EzyArray.h"
#include "EzyPrimitive.h"
#include "EzyString.h"
#include "../rapidjson/rapidjson.h"
#include "../rapidjson/document.h"
#include "../rapidjson/stringbuffer.h"
#include "../rapidjson/writer.h"

EZY_NAMESPACE_START
namespace entity {

inline bool __checkDoubleIsFloat(double d){
	if (d < static_cast<double>(-std::numeric_limits<float>::max()) || d > static_cast<double>(std::numeric_limits<float>::max())){
		return false;
	}
	double b = static_cast<double>(static_cast<float>(d));
	return d >= b && d <= b;
}

inline EzyValue* __createDictFromJson(const rapidjson::Value& value);
inline EzyValue* __createArrayFromJson(const rapidjson::Value& value);
inline EzyValue* __createValueFromJson(const rapidjson::Value& value){
	int type = value.GetType();
	switch (type)
	{
	case rapidjson::Type::kNullType:{
        auto pret = new EzyValue();
        pret->autorelease();
        return pret;
	}
	case rapidjson::Type::kFalseType:{
		auto pret = new EzyPrimitive();
        pret->autorelease();
		pret->setBool(false);
		return pret;
	}
	case rapidjson::Type::kTrueType:{
		auto pret = new EzyPrimitive();
        pret->autorelease();
		pret->setBool(true);
		return pret;
	}
	case rapidjson::Type::kNumberType:{
		auto pret = new EzyPrimitive();
        pret->autorelease();
		if (value.IsInt()){
			pret->setInt(value.GetInt());
		}
		else if (value.IsInt64()){
			pret->setInt(value.GetInt64());
		}
		else if (value.IsUint()){
			pret->setUInt(value.GetUint());
		}
		else if (value.IsUint64()){
			pret->setUInt(value.GetUint64());
		}
		else if (value.IsDouble()){
			double d = value.GetDouble();
			if (__checkDoubleIsFloat(d)){
				pret->setFloat((float)d);
			}
			else{
				pret->setDouble(d);
			}
		}
		return pret;
	}
	case rapidjson::Type::kStringType:{
		auto pret = new entity::EzyString();
        pret->autorelease();
		pret->setString(value.GetString());
		//pret->setData(value.GetString(), value.GetStringLength());
		return pret;
	}
	case rapidjson::Type::kObjectType:{
		return __createDictFromJson(value);
	}
	case rapidjson::Type::kArrayType:{
		return __createArrayFromJson(value);
	}
	}
    
    auto pret = new EzyValue();
    pret->autorelease();
    return pret;
}

inline EzyValue* __createDictFromJson(const rapidjson::Value& value){
	EzyObject* pret = new EzyObject();
    pret->autorelease();
	for (auto it = value.MemberBegin(); it != value.MemberEnd(); ++it){
		EzyValue* item = __createValueFromJson(it->value);
		pret->addItem(it->name.GetString(), item);
		//item->release();
	}
	return pret;
}

inline EzyValue* __createArrayFromJson(const rapidjson::Value& value){
	EzyArray* pret = new EzyArray();
    pret->autorelease();
	for (int i = 0; i < value.Size(); i++){
		EzyValue* item = __createValueFromJson(value[i]);
		pret->addItem(item);
		//item->release();
	}
	return pret;
}

/****/

EzyJson::EzyJson(){
    mValue = 0;
    mString = "";
    mValueType = EzyValueType::TypeJSON;
}

EzyJson::~EzyJson(){
    EZY_SAFE_DELETE(mValue)
}

void EzyJson::writeToBuffer(codec::EzyDataEncoder* encoder) {
	rapidjson::Document doc;
	if (doc.Parse<0>(mString.c_str()).HasParseError()){
		//error parse json
	}
	else{
        EZY_SAFE_DELETE(mValue)
		mValue = __createValueFromJson(doc);
        mValue->retain();
		mValue->writeToBuffer(encoder);
	}
}

#ifdef EZY_DEBUG
void EzyJson::printDebug(){
	if (mValue){
		mValue->printDebug();
	}
	else{
		EzyValue::printDebug();
	}
}
#endif

const std::string& EzyJson::toString(){
	return mString;
}

EzyObject* EzyJson::getValue(){
	return (EzyObject*)mValue;
}

void EzyJson::initWithString(const std::string& json){
	this->mString = json;
}

void EzyJson::initWithValue(EzyValue* value){
    EZY_SAFE_DELETE(mValue)
	mValue = value;
	mValue->retain();
	mString = mValue->toJson();
}

EzyJson* EzyJson::create(const std::string& json){
	auto* pret = new EzyJson();
	pret->initWithString(json);
	pret->autorelease();
	return pret;
}

EzyJson* EzyJson::create(entity::EzyValue* value){
	auto* pret = new EzyJson();
	pret->initWithValue(value);
	pret->autorelease();
	return pret;
}

}
EZY_NAMESPACE_END
