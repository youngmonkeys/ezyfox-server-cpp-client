#include <iomanip>
#include "EzyValue.h"
#include "EzyPrimitive.h"
#include "EzyArray.h"
#include "EzyObject.h"
#include "../logger/EzyLogger.h"

EZY_NAMESPACE_START_WITH(entity)
	
EzyValue::EzyValue() {
	mValueType = EzyValueType::TypeNULL;
}

EzyValue::~EzyValue() {
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

void EzyValue::refreshLogBuffer(std::ostringstream& stream) {
	logger::console(stream.str().c_str());
	stream.str("");
	stream.clear();
}

void EzyValue::printToOutStream(std::ostringstream& stream, int padding) {
	stream << "[NULL]";
}

void EzyValue::printPadding(std::ostringstream& stream, int padding) {
	for (int i = 0; i < padding; ++i) {
#if defined(ANDROID)
		stream << "  ";
#else
		stream << "\t";
#endif	
	}
}
#endif

int EzyValue::getType() {
    return mValueType;
}

EZY_NAMESPACE_END_WITH
