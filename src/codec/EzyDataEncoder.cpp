/*
 * EzyDataEncoder.cpp
 *
 *  Created on: Jun 10, 2016
 *      Author: Quyet Nguyen
 */

#include "EzyDataEncoder.h"

EZY_NAMESPACE_START
namespace codec {
	
EzyDataEncoder::EzyDataEncoder() {
	// TODO Auto-generated constructor stub
	mBuffer.reserve(102400); // 100KB Buffer
}

EzyDataEncoder::~EzyDataEncoder() {
	// TODO Auto-generated destructor stub
}

const std::vector<char>& EzyDataEncoder::getBuffer() {
	return mBuffer;
}

void EzyDataEncoder::clear() {
	mBuffer.clear();
}

inline void __writer_swap_bytes(char* bytes, int size){
	for (int i = 0, j = size - 1; i < j; i++, j--){
		char c = bytes[i];
		bytes[i] = bytes[j];
		bytes[j] = c;
	}
}

void EzyDataEncoder::write_native_Bytes(const char* bytes, int32_t size){
	mBuffer.insert(mBuffer.end(), bytes, bytes + size);
}

void EzyDataEncoder::write_native_Int8(int8_t value){
	write_native_Bytes((const char*)&value, 1);
}

void EzyDataEncoder::write_native_Int16(int16_t value){
	__writer_swap_bytes((char*)&value, 2);
	write_native_Bytes((const char*)&value, 2);
}

void EzyDataEncoder::write_native_Int32(int32_t value){
	__writer_swap_bytes((char*)&value, 4);
	write_native_Bytes((const char*)&value, 4);
}

void EzyDataEncoder::write_native_Int64(int64_t value){
	__writer_swap_bytes((char*)&value, 8);
	write_native_Bytes((const char*)&value, 8);
}

void EzyDataEncoder::write_native_UInt8(uint8_t value){
	write_native_Bytes((const char*)&value, 1);
}

void EzyDataEncoder::write_native_UInt16(uint16_t value){
	__writer_swap_bytes((char*)&value, 2);
	write_native_Bytes((const char*)&value, 2);
}

void EzyDataEncoder::write_native_UInt32(uint32_t value){
	__writer_swap_bytes((char*)&value, 4);
	write_native_Bytes((const char*)&value, 4);
}

void EzyDataEncoder::write_native_UInt64(uint64_t value){
	__writer_swap_bytes((char*)&value, 8);
	write_native_Bytes((const char*)&value, 8);
}

void EzyDataEncoder::write_native_float(float f){
	__writer_swap_bytes((char*)&f, 4);
	write_native_Bytes((const char*)&f, 4);
}

void EzyDataEncoder::write_native_double(double d){
	__writer_swap_bytes((char*)&d, 8);
	write_native_Bytes((const char*)&d, 8);
}

void EzyDataEncoder::writeNil(){
	write_native_UInt8(0xc0);
}

void EzyDataEncoder::writeBool(bool b){
	if (b){
		write_native_UInt8(0xc3);
	}
	else{
		write_native_UInt8(0xc2);
	}
	
}

void EzyDataEncoder::writeUint(uint64_t i64){
	if (i64 < 0x7f){ //fix int
		write_native_UInt8((uint8_t)i64);
	}
	else if (i64 <= UINT8_MAX){
		write_native_UInt8(0xcc);
		write_native_UInt8((uint8_t)i64);
	}
	else if (i64 <= UINT16_MAX){
		write_native_UInt8(0xcd);
		write_native_UInt16((uint16_t)i64);
	}
	else if (i64 <= UINT32_MAX){
		write_native_UInt8(0xce);
		write_native_UInt32((uint32_t)i64);
	}
	else{
		write_native_UInt8(0xcf);
		write_native_UInt64((uint64_t)i64);
	}
}

void EzyDataEncoder::writeInt(int64_t i64){
	if (i64 >= 0){
		this->writeUint((uint64_t)i64);
	}
	else{
		if (i64 >= -32) { //fix int
			write_native_Int8((int8_t)0xe0 | (int8_t)i64);
		}
		else if (i64 >= INT8_MIN) { //int 8
			write_native_UInt8(0xd0);
			write_native_Int8((int8_t)i64);
		
		}
		else if (i64 >= INT16_MIN) { //int 16
			write_native_UInt8(0xd1);
			write_native_Int16((int16_t)i64);
		}
		else if (i64 >= INT32_MIN) { //int32
			write_native_UInt8(0xd2);
			write_native_Int32((int32_t)i64);
		}
		else {
			write_native_UInt8(0xd3);
			write_native_Int64((int64_t)i64); //int64
		}

	}
}

void EzyDataEncoder::writeFloat(float f){
	write_native_UInt8(0xca);
	write_native_float(f);
}

void EzyDataEncoder::writeDouble(double d){
	write_native_UInt8(0xcb);
	write_native_double(d);
}

void EzyDataEncoder::writeString(const std::string& str){
	uint64_t size = str.size();

	if (size <= 31) {
		write_native_UInt8((uint8_t)(0xa0 | size));
	}
	else if (size <= UINT8_MAX) {
		write_native_UInt8(0xd9);
		write_native_UInt8((uint8_t)size);
	}
	else if (size <= UINT16_MAX) {
		write_native_UInt8(0xda);
		write_native_UInt16((uint16_t)size);
	}
	else {
		write_native_UInt8(0xdb);
		write_native_UInt32((uint32_t)size);
	}

	write_native_Bytes(str.data(), size);
}

void EzyDataEncoder::writeBin(const char* str, uint32_t size){
	if (size <= UINT8_MAX) {
		write_native_UInt8(0xc4);
		write_native_UInt8((uint8_t)size);
	}
	else if (size <= UINT16_MAX) {
		write_native_UInt8(0xc5);
		write_native_UInt16((uint16_t)size);
	}
	else {
		write_native_UInt8(0xc6);
		write_native_UInt32((uint32_t)size);
	}

	write_native_Bytes(str, size);
}

void EzyDataEncoder::writeArray(uint32_t size){
	if (size <= 15){
		write_native_UInt8(0x90 | size);
	}
	else if (size <= UINT16_MAX){
		write_native_UInt8(0xdc);
		write_native_UInt16((uint16_t)size);
	}
	else{
		write_native_UInt8(0xdd);
		write_native_UInt32((uint32_t)size);
	}
}

void EzyDataEncoder::writeMap(uint32_t size){
	if (size <= 15){
		write_native_UInt8(0x80 | size);
	}
	else if (size <= UINT16_MAX){
		write_native_UInt8(0xde);
		write_native_UInt16((uint16_t)size);
	}
	else{
		write_native_UInt8(0xdf);
		write_native_UInt32((uint32_t)size);
	}
}

uint32_t EzyDataEncoder::getSize(){
	return mBuffer.size();
}

void EzyDataEncoder::insertHeader(const char* bytes, int size){
	mBuffer.insert(mBuffer.begin(), bytes, bytes + size);
}

}
EZY_NAMESPACE_END
