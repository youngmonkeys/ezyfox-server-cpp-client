#include "EzyMessage.h"

EZY_NAMESPACE_START
namespace codec {
    
EzyMessageHeader::EzyMessageHeader() {
    mText = false;
    mBigSize = false;
    mEncrypted = false;
    mCompressed = false;
}
    
EzyMessageHeader::EzyMessageHeader(uint32_t dataSize) {
    mText = false;
    mEncrypted = false;
    mCompressed = false;
    mBigSize = dataSize > MAX_SMALL_MESSAGE_SIZE;
}
    
EzyMessageHeader::~EzyMessageHeader() {
}

EzyMessageHeader* EzyMessageHeader::create() {
    auto pret = new EzyMessageHeader();
    pret->autorelease();
    return pret;
}
    
EzyMessageHeader* EzyMessageHeader::create(uint32_t dataSize) {
    auto pret = new EzyMessageHeader(dataSize);
    pret->autorelease();
    return pret;
}
    
void EzyMessageHeader::parse(char headerByte) {
    mBigSize = (headerByte & 1 << 0) > 0;
    mEncrypted = (headerByte & 1 << 1) > 0;
    mCompressed = (headerByte & 1 << 2) > 0;
    mText = (headerByte & 1 << 3) > 0;
}
    
char EzyMessageHeader::getByte() {
    char headerByte = 0;
    headerByte |= mBigSize ? 1 << 0 : 0;
    headerByte |= mEncrypted ? 1 << 1 : 0;
    headerByte |= mCompressed ? 1 << 2 : 0;
    headerByte |= mText ? 1 << 3 : 0;
    return headerByte;
}
    
}
EZY_NAMESPACE_END
