//
//  EzyNull.cpp
//  ezyfox-ssl
//
//  Created by Dzung on 04/06/2021.
//

#include "EzyNull.h"

EZY_NAMESPACE_START_WITH(entity)

EzyNull::EzyNull() {
    mValueType = EzyValueType::TypeNULL;
}

EzyNull::~EzyNull() {
}

void EzyNull::writeToBuffer(codec::EzyDataEncoder* encoder) {
    encoder->writeNil();
}

#ifdef EZY_DEBUG
void EzyNull::printToOutStream(std::ostringstream& stream, int padding) {
    stream << "[Null]";
}
#endif

std::string EzyNull::toString() const {
    return "Null";
}

EZY_NAMESPACE_END_WITH

