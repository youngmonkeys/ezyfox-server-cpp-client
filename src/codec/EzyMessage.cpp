#include "EzyMessage.h"

EZY_NAMESPACE_START_WITH(codec)
    
EzyMessageHeader::EzyMessageHeader() {
    mText = false;
    mBigSize = false;
    mEncrypted = false;
    mCompressed = false;
    mRawBytes = false;
    mUdpHandshake = false;
    mHasNext = false;
}
    
EzyMessageHeader::EzyMessageHeader(uint32_t dataSize, bool encrypted) {
    mText = false;
    mEncrypted = encrypted;
    mCompressed = false;
    mBigSize = dataSize > MAX_SMALL_MESSAGE_SIZE;
    mRawBytes = false;
    mUdpHandshake = false;
    mHasNext = false;
}
    
EzyMessageHeader::~EzyMessageHeader() {
}

EzyMessageHeader* EzyMessageHeader::create() {
    auto pret = new EzyMessageHeader();
    pret->autorelease();
    return pret;
}
    
EzyMessageHeader* EzyMessageHeader::create(uint32_t dataSize, bool encrypted) {
    auto pret = new EzyMessageHeader(dataSize, encrypted);
    pret->autorelease();
    return pret;
}
    
void EzyMessageHeader::parse(char headerByte) {
    mBigSize        = (headerByte & 1 << 0) > 0;
    mEncrypted      = (headerByte & 1 << 1) > 0;
    mCompressed     = (headerByte & 1 << 2) > 0;
    mText           = (headerByte & 1 << 3) > 0;
    mRawBytes       = (headerByte & 1 << 4) > 0;
    mUdpHandshake   = (headerByte & 1 << 5) > 0;
    mHasNext        = (headerByte & 1 << 7) > 0;
}
    
char EzyMessageHeader::getByte() {
    char headerByte = 0;
    headerByte      |= mBigSize      ? 1 << 0 : 0;
    headerByte      |= mEncrypted    ? 1 << 1 : 0;
    headerByte      |= mCompressed   ? 1 << 2 : 0;
    headerByte      |= mText         ? 1 << 3 : 0;
    headerByte      |= mRawBytes     ? 1 << 4 : 0;
    headerByte      |= mUdpHandshake ? 1 << 5 : 0;
    headerByte      |= mHasNext      ? 1 << 7 : 0;
    return headerByte;
}

EZY_NAMESPACE_END_WITH
