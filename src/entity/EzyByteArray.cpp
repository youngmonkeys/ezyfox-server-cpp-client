#include <string>
#include "EzyByteArray.h"

EZY_NAMESPACE_START_WITH(entity)

EzyByteArray::EzyByteArray() {
    mValueType = EzyValueType::TypeByteArray;
    mData = "";
}

EzyByteArray::~EzyByteArray() {
}

void EzyByteArray::writeToBuffer(codec::EzyDataEncoder* encoder) {
    encoder->writeBin(mData.c_str(), (uint32_t)mData.size());
}
#ifdef EZY_DEBUG
void EzyByteArray::printToOutStream(std::ostringstream& stream, int padding) {
    stream << "[ByteArray] " << "(" << mData.size() << ")" <<" [ ";
    for(int i = 0 ; i < mData.size() ; ++i) {
        stream << std::to_string((int8_t)mData[i]) << ' ';
    }
    stream << "]\n";
}
#endif

void EzyByteArray::setData(std::string data) {
    mData = data;
}

void EzyByteArray::setData(const char* buffer, int size) {
    if (size > 0) {
        mData.assign(buffer, buffer + size);
    }
}

const std::string EzyByteArray::getData() {
    return mData;
}

std::string EzyByteArray::toString() const {
    return std::string("\"").append(mData).append("\"");
}

EZY_NAMESPACE_END_WITH
