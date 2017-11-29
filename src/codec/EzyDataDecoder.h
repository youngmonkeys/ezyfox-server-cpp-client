#pragma once

#include <vector>
#include <cstdint>
#include <string>
#include <stack>
#include "../entity/EzyValue.h"

EZY_NAMESPACE_START
namespace codec {
    
class EzyArrayBuffer {
private:
	int mType;
	int mSize;
	std::vector<entity::EzyValue*> mArray;
public:
	EzyArrayBuffer(int type, int size);
	virtual ~EzyArrayBuffer();
    
    void pushValue(entity::EzyValue* value);
    bool validate();
    
    entity::EzyValue* toValue();
};

class EzyDataDecoderDelegate {
public:
    virtual void onRecvMessage(entity::EzyValue* value) = 0;
};

class EzyDataDecoder {
	EzyDataDecoderDelegate* mDelegate;

	std::vector<char> mDataBuffer;

	int processData(const char* buffer, int& dataSize);

	uint8_t		readUInt8(const char* p);
	uint16_t	readUInt16(const char* p);
	uint32_t	readUInt32(const char* p);
	uint64_t	readUInt64(const char* p);
	int8_t		readInt8(const char* p);
	int16_t		readInt16(const char* p);
	int32_t		readInt32(const char* p);
	int64_t		readInt64(const char* p);
	float		readFloat(const char* p);
	double		readDouble(const char* p);

	void onReadNil();
	void onReadBool(bool b);

	void onReadInt(int64_t i64);
	void onReadUnsignedInt(uint64_t i64);

	void onReadFloat(float f);
	void onReadDouble(double d);

	void onReadString(const char* str, uint32_t size);
	void onReadBin(const char* str, uint32_t size);

	void onReadMap(uint32_t size);
	void onReadArray(uint32_t size);

	std::stack<EzyArrayBuffer*> mStack;
	void onReadValue(entity::EzyValue* object);
public:
	EzyDataDecoder();
	virtual ~EzyDataDecoder();

	virtual void setDelegate(EzyDataDecoderDelegate* mDelegate);

	void addData(const char* data, int size);
	void addData(const std::vector<char> data);
};

}
EZY_NAMESPACE_END
