#include "EzyString.h"

EZY_NAMESPACE_START
namespace entity {

EzyString::EzyString() {
    valueType = EzyValueType::TypeString;
    data = "";
}

EzyString::~EzyString(){
    
}

void EzyString::writeToBuffer(codec::EzyDataEncoder* encoder) {
    encoder->writeString(data);
}
#ifdef EZY_DEBUG
void EzyString::printToOutStream(std::ostringstream& stream, int padding) {
    stream << "[String] " << data;
}
#endif

void EzyString::toValue(rapidjson::Value& value, rapidjson::Document::AllocatorType& allocator) {
    value.SetString(data, allocator);
}

void EzyString::setString(const std::string& str) {
    data = str;
}

void EzyString::setData(const char* buffer, int size) {
    //data = std::string(buffer, size);
    if (size > 0){
        data.assign(buffer, buffer + size);
    }
}

const std::string& EzyString::getString() {
    return data;
}

}
EZY_NAMESPACE_END
