#pragma once

#ifndef __STDINT_LIMITS
#define __STDINT_LIMITS
#endif
#include <cstdint>
#include <vector>
#include <string>
#include <stack>
#include "../EzyMacro.h"
#include "EzyMessage.h"

EZY_NAMESPACE_START
namespace codec {

class EzyDataEncoder {
protected:
    
    std::vector<char> mBuffer;
    
	void writeNativeBytes(const char* bytes, int32_t size);
	void writeNativeInt8(int8_t value);
	void writeNativeInt16(int16_t value);
	void writeNativeInt32(int32_t value);
	void writeNativeInt64(int64_t value);
	void writeNativeUInt8(uint8_t value);
	void writeNativeUInt16(uint16_t value);
	void writeNativeUInt32(uint32_t value);
	void writeNativeUInt64(uint64_t value);
	void writeNativeFloat(float f);
	void writeNativeDouble(double d);
    
    char getHeader(uint32_t data);
public:
	EzyDataEncoder();
	virtual ~EzyDataEncoder();

	const std::vector<char>& getBuffer();
	void clear();

	void writeNil();
	void writeBool(bool b);
	void writeUint(uint64_t i64);
	void writeInt(int64_t i64);
	void writeFloat(float f);
	void writeDouble(double d);
	void writeString(const std::string& str);
	void writeBin(const char* str, uint32_t size);
	void writeArray(uint32_t size);
	void writeMap(uint32_t size);

	uint32_t getSize();
    void insertHeader(EzyMessageHeader* header);
	void insertDataSize(EzyMessageHeader* header, uint32_t dataSize);
};

}
EZY_NAMESPACE_END
