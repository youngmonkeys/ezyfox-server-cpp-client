#include <iomanip>
#include "EzyValue.h"
#include "EzyPrimitive.h"
#include "EzyArray.h"
#include "EzyObject.h"
#include "../logger/EzyLogger.h"
#include "../rapidjson/stringbuffer.h"
#include "../rapidjson/prettywriter.h"

EZY_NAMESPACE_START
namespace entity {
	
EzyValue::EzyValue() {
	// TODO Auto-generated constructor stub
	valueType = EzyValueType::TypeNULL;
}

EzyValue::~EzyValue() {
	// TODO Auto-generated destructor stub
//	logger::log("delete Value[%d]", valueType);
}

void EzyValue::writeToBuffer(codec::EzyDataEncoder* encoder) {
	encoder->writeNil();
}

#ifdef EZY_DEBUG
void EzyValue::printDebug() {
	std::ostringstream stream;
	this->printToOutStream(stream, 0);
	refreshLogBuffer(stream);
}

void EzyValue::refreshLogBuffer(std::ostringstream& stream){
	logger::console(stream.str().c_str());
	stream.str("");
	stream.clear();
}

void EzyValue::printToOutStream(std::ostringstream& stream, int padding){
	stream << "[NULL]";
}

void EzyValue::printPadding(std::ostringstream& stream, int padding){
	for (int i = 0; i < padding; i++){
#if defined(ANDROID)
		stream << "  ";
#else
		stream << "\t";
#endif	
	}
}
#endif

void EzyValue::toValue(rapidjson::Value& value, rapidjson::Document::AllocatorType& allocator){
	value.SetNull();
}

std::string EzyValue::toJson() {

	rapidjson::Document doc;
	this->toValue(doc, doc.GetAllocator());

	rapidjson::StringBuffer buffer;
	buffer.Clear();
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	doc.Accept(writer);
	std::string json = buffer.GetString();
	return json;
}

}
EZY_NAMESPACE_END
