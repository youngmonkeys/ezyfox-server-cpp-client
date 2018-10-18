#include "EzyDataEncoder.h"

EZY_NAMESPACE_START_WITH(codec)
	
EzyDataEncoder::EzyDataEncoder() {
	mBuffer.reserve(102400); // 100KB Buffer
}

EzyDataEncoder::~EzyDataEncoder() {
}

const std::vector<char>& EzyDataEncoder::getBuffer() {
	return mBuffer;
}

void EzyDataEncoder::clear() {
	mBuffer.clear();
}

inline void __swapBytes(char* bytes, int size){
	for (int i = 0, j = size - 1; i < j; i++, j--){
		char c = bytes[i];
		bytes[i] = bytes[j];
		bytes[j] = c;
	}
}

void EzyDataEncoder::writeNativeBytes(const char* bytes, int32_t size){
	mBuffer.insert(mBuffer.end(), bytes, bytes + size);
}

void EzyDataEncoder::writeNativeInt8(int8_t value){
	writeNativeBytes((const char*)&value, 1);
}

void EzyDataEncoder::writeNativeInt16(int16_t value){
	__swapBytes((char*)&value, 2);
	writeNativeBytes((const char*)&value, 2);
}

void EzyDataEncoder::writeNativeInt32(int32_t value){
	__swapBytes((char*)&value, 4);
	writeNativeBytes((const char*)&value, 4);
}

void EzyDataEncoder::writeNativeInt64(int64_t value){
	__swapBytes((char*)&value, 8);
	writeNativeBytes((const char*)&value, 8);
}

void EzyDataEncoder::writeNativeUInt8(uint8_t value){
	writeNativeBytes((const char*)&value, 1);
}

void EzyDataEncoder::writeNativeUInt16(uint16_t value){
	__swapBytes((char*)&value, 2);
	writeNativeBytes((const char*)&value, 2);
}

void EzyDataEncoder::writeNativeUInt32(uint32_t value){
	__swapBytes((char*)&value, 4);
	writeNativeBytes((const char*)&value, 4);
}

void EzyDataEncoder::writeNativeUInt64(uint64_t value){
	__swapBytes((char*)&value, 8);
	writeNativeBytes((const char*)&value, 8);
}

void EzyDataEncoder::writeNativeFloat(float f){
	__swapBytes((char*)&f, 4);
	writeNativeBytes((const char*)&f, 4);
}

void EzyDataEncoder::writeNativeDouble(double d){
	__swapBytes((char*)&d, 8);
	writeNativeBytes((const char*)&d, 8);
}

void EzyDataEncoder::writeNil(){
	writeNativeUInt8(0xc0);
}

void EzyDataEncoder::writeBool(bool b){
	if (b){
		writeNativeUInt8(0xc3);
	}
	else{
		writeNativeUInt8(0xc2);
	}
	
}

void EzyDataEncoder::writeUint(uint64_t i64){
	if (i64 < 0x7f){ //fix int
		writeNativeUInt8((uint8_t)i64);
	}
	else if (i64 <= UINT8_MAX){
		writeNativeUInt8(0xcc);
		writeNativeUInt8((uint8_t)i64);
	}
	else if (i64 <= UINT16_MAX){
		writeNativeUInt8(0xcd);
		writeNativeUInt16((uint16_t)i64);
	}
	else if (i64 <= UINT32_MAX){
		writeNativeUInt8(0xce);
		writeNativeUInt32((uint32_t)i64);
	}
	else{
		writeNativeUInt8(0xcf);
		writeNativeUInt64((uint64_t)i64);
	}
}

void EzyDataEncoder::writeInt(int64_t i64){
	if (i64 >= 0){
		this->writeUint((uint64_t)i64);
	}
	else{
		if (i64 >= -32) { //fix int
			writeNativeInt8((int8_t)0xe0 | (int8_t)i64);
		}
		else if (i64 >= INT8_MIN) { //int 8
			writeNativeUInt8(0xd0);
			writeNativeInt8((int8_t)i64);
		
		}
		else if (i64 >= INT16_MIN) { //int 16
			writeNativeUInt8(0xd1);
			writeNativeInt16((int16_t)i64);
		}
		else if (i64 >= INT32_MIN) { //int32
			writeNativeUInt8(0xd2);
			writeNativeInt32((int32_t)i64);
		}
		else {
			writeNativeUInt8(0xd3);
			writeNativeInt64((int64_t)i64); //int64
		}

	}
}

void EzyDataEncoder::writeFloat(float f){
	writeNativeUInt8(0xca);
	writeNativeFloat(f);
}

void EzyDataEncoder::writeDouble(double d){
	writeNativeUInt8(0xcb);
	writeNativeDouble(d);
}

void EzyDataEncoder::writeString(const std::string& str){
	uint64_t size = str.size();

	if (size <= 31) {
		writeNativeUInt8((uint8_t)(0xa0 | size));
	}
	else if (size <= UINT8_MAX) {
		writeNativeUInt8(0xd9);
		writeNativeUInt8((uint8_t)size);
	}
	else if (size <= UINT16_MAX) {
		writeNativeUInt8(0xda);
		writeNativeUInt16((uint16_t)size);
	}
	else {
		writeNativeUInt8(0xdb);
		writeNativeUInt32((uint32_t)size);
	}

	writeNativeBytes(str.data(), (int32_t)size);
}

void EzyDataEncoder::writeBin(const char* str, uint32_t size){
	if (size <= UINT8_MAX) {
		writeNativeUInt8(0xc4);
		writeNativeUInt8((uint8_t)size);
	}
	else if (size <= UINT16_MAX) {
		writeNativeUInt8(0xc5);
		writeNativeUInt16((uint16_t)size);
	}
	else {
		writeNativeUInt8(0xc6);
		writeNativeUInt32((uint32_t)size);
	}

	writeNativeBytes(str, size);
}

void EzyDataEncoder::writeArray(uint32_t size){
	if (size <= 15){
		writeNativeUInt8(0x90 | size);
	}
	else if (size <= UINT16_MAX){
		writeNativeUInt8(0xdc);
		writeNativeUInt16((uint16_t)size);
	}
	else{
		writeNativeUInt8(0xdd);
		writeNativeUInt32((uint32_t)size);
	}
}

void EzyDataEncoder::writeMap(uint32_t size){
	if (size <= 15){
		writeNativeUInt8(0x80 | size);
	}
	else if (size <= UINT16_MAX){
		writeNativeUInt8(0xde);
		writeNativeUInt16((uint16_t)size);
	}
	else{
		writeNativeUInt8(0xdf);
		writeNativeUInt32((uint32_t)size);
	}
}

uint32_t EzyDataEncoder::getSize(){
	return (uint32_t)mBuffer.size();
}

void EzyDataEncoder::insertHeader(EzyMessageHeader* header) {
    auto headerByte = header->getByte();
    const char* bytes = (const char*) &headerByte;
	mBuffer.insert(mBuffer.begin(), bytes, bytes + 1);
}
    
void EzyDataEncoder::insertDataSize(EzyMessageHeader* header, uint32_t dataSize) {
    auto size = header->isBigSize() ? 4 : 2;
    const char* bytes = (const char*) &dataSize;
    mBuffer.insert(mBuffer.begin(), bytes, bytes + size);
}

EZY_NAMESPACE_END_WITH
