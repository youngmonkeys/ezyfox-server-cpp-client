#include "EzyString.h"

EZY_NAMESPACE_START_WITH(entity)

EzyString::EzyString() {
    mData = "";
    mValueType = EzyValueType::TypeString;
}

EzyString::~EzyString() {
    
}

void EzyString::writeToBuffer(codec::EzyDataEncoder* encoder) {
    encoder->writeString(mData);
}
#ifdef EZY_DEBUG
void EzyString::printToOutStream(std::ostringstream& stream, int padding) {
    stream << "[String] " << mData;
}
#endif

void EzyString::setString(const std::string& str) {
    mData = str;
}

void EzyString::setData(const char* buffer, int size) {
    if (size > 0) {
        mData.assign(buffer, buffer + size);
    }
}

const std::string& EzyString::getString() {
    return mData;
}

std::string EzyString::toString() const {
    return std::string("\"").append(mData).append("\"");
}

EZY_NAMESPACE_END_WITH
