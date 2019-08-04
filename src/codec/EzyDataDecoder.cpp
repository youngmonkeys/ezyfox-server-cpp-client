#include "EzyDataDecoder.h"
#include "EzyPrimitive.h"
#include "EzyArray.h"
#include "EzyObject.h"
#include "EzyString.h"

EZY_NAMESPACE_START_WITH(codec)

EzyArrayBuffer::EzyArrayBuffer(int type, int size) {
	this->mSize = size;
    this->mType = type;
	mArray.reserve(size);
}

EzyArrayBuffer::~EzyArrayBuffer() {
	mArray.clear();
}
    
void EzyArrayBuffer::pushValue(entity::EzyValue* value) {
    mArray.push_back(value);
}

bool EzyArrayBuffer::validate() {
    return (mArray.size() >= mSize);
}

entity::EzyValue* EzyArrayBuffer::toValue() {
    if(!validate()) {
        return 0;
    }
    if(mType == entity::EzyValueType::TypeArray) {
        auto newValue = new entity::EzyArray();
        for (int i = 0; i < mArray.size(); ++i) {
            newValue->addItem(mArray[i]);
        }
        return newValue;
    }
    else if(mType == entity::EzyValueType::TypeDict) {
        auto newValue = new entity::EzyObject();
        for (int i = 0; i < mArray.size(); i += 2) {
            auto key = mArray[i];
            auto value = mArray[i + 1];
            newValue->addItem(((entity::EzyString*)key)->getString(), value);
            key->autorelease();
        }
        return newValue;
    }
    return 0;
}

/****/

EzyDataDecoder::EzyDataDecoder() {
	mDataBuffer.reserve(16 * 1024); //16KB buffer
	mDelegate = 0;
}

EzyDataDecoder::~EzyDataDecoder() {
	while (!mStack.empty()) {
		auto item = mStack.top();
		delete item;
		mStack.pop();
	}
}

void EzyDataDecoder::setDelegate(EzyDataDecoderDelegate* delegate) {
	mDelegate = delegate;
}

void EzyDataDecoder::addData(const char* data, int size) {
	mDataBuffer.insert(mDataBuffer.end(), data, data + size);

	auto buffer = mDataBuffer.data();
	int dataSize = (int)mDataBuffer.size();
	while (true) {
		auto n = processData(buffer, dataSize);
		if (n <= 0) {
			break;
		}

		buffer += n;
		dataSize -= n;
	}
	

	int lastIndex = (int)(mDataBuffer.size() - dataSize);
	mDataBuffer.erase(mDataBuffer.begin(), mDataBuffer.begin() + lastIndex);
}

void EzyDataDecoder::addData(const std::vector<char> data) {
	this->addData(data.data(), (int)data.size());
}

/****/
inline void __swapBytes(char* bytes, int size) {
    for (int i = 0, j = size - 1; i < j; ++i, --j) {
        char c = bytes[i];
        bytes[i] = bytes[j];
        bytes[j] = c;
    }
}

uint8_t	EzyDataDecoder::readUInt8(const char* p) {
	return (uint8_t)p[0];
}

uint16_t EzyDataDecoder::readUInt16(const char* p) {
	uint16_t pret;
	memcpy(&pret, p, 2);
	__swapBytes((char*)&pret, 2);
	return pret;
}

uint32_t EzyDataDecoder::readUInt32(const char* p) {
	uint32_t pret;
	memcpy(&pret, p, 4);
	__swapBytes((char*)&pret, 4);
	return pret;
}

uint64_t EzyDataDecoder::readUInt64(const char* p) {
	uint64_t pret;
	memcpy(&pret, p, 8);
	__swapBytes((char*)&pret, 8);
	return pret;
}

int8_t	EzyDataDecoder::readInt8(const char* p) {
	return (int8_t)p[0];
}

int16_t	EzyDataDecoder::readInt16(const char* p) {
	int16_t pret;
	memcpy(&pret, p, 2);
	__swapBytes((char*)&pret, 2);
	return pret;
}

int32_t	EzyDataDecoder::readInt32(const char* p) {
	int32_t pret;
	memcpy(&pret, p, 4);
	__swapBytes((char*)&pret, 4);
	return pret;
}

int64_t	EzyDataDecoder::readInt64(const char* p) {
	int64_t pret;
	memcpy(&pret, p, 8);
	__swapBytes((char*)&pret, 8);
	return pret;
}

float	EzyDataDecoder::readFloat(const char* p) {
	float pret;
	memcpy(&pret, p, 4);
	__swapBytes((char*)&pret, 4);
	return pret;
}

double	EzyDataDecoder::readDouble(const char* p) {
	double pret;
	memcpy(&pret, p, 8);
	__swapBytes((char*)&pret, 8);
	return pret;
}

int EzyDataDecoder::processData(const char* buffer, int& dataSize) {
	if (dataSize <= 0) {
		return 0;
	}
	const unsigned char type = (const unsigned char)buffer[0];

	if (0x00 <= type && type <= 0x7f) {
		// positive fixnum
		this->onReadUnsignedInt((uint8_t)type);
		return 1;
	}
	else if (0xe0 <= type && type <= 0xff) {
		// negative fixnum
		this->onReadInt((int8_t)type);
		return 1;
	}
	else if (0x80 <= type && type <= 0x8f) {
		// fixmap
		unsigned char n = (type & 0x0f);
		this->onReadMap(n);
		return 1;
	}
	else if (0x90 <= type && type <= 0x9f) {
		//fixarray
		unsigned char n = (type & 0x0f);
		this->onReadArray(n);
		return 1;
	}
	else if (0xa0 <= type && type <= 0xbf) {
		//fixstr
		unsigned char n = (type & 0x1f);// (type & ~0xe0);
		if (dataSize >= n + 1) {
			this->onReadString(buffer + 1, n);
			return (n + 1);
		}
		return 0;
	}
	else if (type == 0xc0) {
		//nil
		this->onReadNil();
		return 1;
	}
	else if (0xc2 <= type && type <= 0xc3) {
		//bool
		bool b = type & 1;
		this->onReadBool(b);
		return 1;
	}
	else if (type == 0xd9) {
		//str 8
		if (dataSize >= 2) {
			uint8_t n = readUInt8(buffer + 1);
			if (dataSize >= n + 2) {
				this->onReadString(buffer + 2, n);
				return (n + 2);
			}
		}
		return 0;
	}
	else if (type == 0xda) {
		//str 16
		if (dataSize >= 3) {
			uint16_t n = readUInt16(buffer + 1);
			if (dataSize >= n + 3) {
				this->onReadString(buffer + 3, n);
				return (n + 3);
			}
		}
		return 0;
	}
	else if (type == 0xdb) {
		//str 32
		if (dataSize >= 5) {
			uint32_t n = readUInt32(buffer + 1);
			if (dataSize >= n + 5) {
				this->onReadString(buffer + 5, n);
				return (n + 5);
			}
		}
		return 0;
	}
	else if (type == 0xd0) {
		//int 8
		if (dataSize >= 2) {
			int8_t uintData = readInt8(buffer + 1);
			this->onReadInt(uintData);
			return 2;
		}
		return 0;
	}
	else if (type == 0xd1) {
		//int 16
		if (dataSize >= 3) {
			int16_t uintData = readInt16(buffer + 1);
			this->onReadInt(uintData);
			return 3;
		}
		return 0;
	}
	else if (type == 0xd2) {
		//int 32
		if (dataSize >= 5) {
			int64_t uintData = readInt32(buffer + 1);
			this->onReadInt(uintData);
			return 5;
		}
		return 0;
	}
	else if (type == 0xd3) {
		//int 64
		if (dataSize >= 9) {
			int64_t uintData = readInt64(buffer + 1);
			this->onReadInt(uintData);
			return 9;
		}
		return 0;
	}	
	else if (type == 0xcc) {
		//uint 8
		if (dataSize >= 2) {
			uint8_t uintData = readUInt8(buffer + 1);
			this->onReadUnsignedInt(uintData);
			return 2;
		}
		return 0;
	}
	else if (type == 0xcd) {
		//uint 16
		if (dataSize >= 3) {
			uint16_t uintData = readUInt16(buffer + 1);
			this->onReadUnsignedInt(uintData);
			return 3;
		}
		return 0;
	}
	else if (type == 0xce) {
		//uint 32
		if (dataSize >= 5) {
			uint32_t uintData = readUInt32(buffer + 1);
			this->onReadUnsignedInt(uintData);
			return 5;
		}
		return 0;
	}
	else if (type == 0xcf) {
		//uint 64
		if (dataSize >= 9) {
			uint64_t uintData = readUInt64(buffer + 1);
			this->onReadUnsignedInt(uintData);
			return 9;
		}
		return 0;
	}
	else if (type == 0xca) {
		//float 32
		if (dataSize >= 5) {
			float f = readFloat(buffer + 1);
			this->onReadFloat(f);
			return 5;
		}
		return 0;
	}
	else if (type == 0xcb) {
		//float 64
		if (dataSize >= 9) {
			double d = readDouble(buffer + 1);
			this->onReadFloat(d);
			return 9;
		}
		return 0;
	}
	else if (type == 0xc4) {
		//bin 8
		if (dataSize >= 2) {
			int n = readUInt8(buffer + 1);
			if (dataSize >= n + 2) {
				this->onReadBin(buffer + 2, n);
				return (n + 2);
			}
		}
		return 0;
	}
	else if (type == 0xc5) {
		//bin 16
		if (dataSize >= 3) {
			int n = readUInt16(buffer + 1);
			if (dataSize >= n + 3) {
				this->onReadBin(buffer + 3, n);
				return (n + 3);
			}
		}
		return 0;
	}
	else if (type == 0xc6) {
		//bin 32
		if (dataSize >= 5) {
			int n = readUInt32(buffer + 1);
			if (dataSize >= n + 5) {
				this->onReadBin(buffer + 5, n);
				return (n + 5);
			}
		}
		return 0;
	}
	else if (type == 0xdc) {
		//array 16
		if (dataSize >= 3) {
			uint16_t n = readUInt16(buffer + 1);
			this->onReadArray(n);
			return 3;
		}
		return 0;
	}
	else if (type == 0xdd) {
		//array 32
		if (dataSize >= 5) {
			uint32_t n = readUInt32(buffer + 1);
			this->onReadArray(n);
			return 5;
		}
		return 0;
	}
	else if (type == 0xde) {
		//map 16
		if (dataSize >= 3) {
			uint16_t n = readUInt16(buffer + 1);
			this->onReadMap(n);
			return 3;
		}
		return 0;
	}
	else if (type == 0xdf) {
		//map 32
		if (dataSize >= 5) {
			uint32_t n = readUInt32(buffer + 1);
			this->onReadMap(n);
			return 5;
		}
		return 0;
	}
	else if (type == 0xd4) {
		//fixext 1
		return 0;
	}

	else if (type == 0xd5) {
		//fixext 2
		return 0;
	}
	else if (type == 0xd6) {
		//fixext 4
		return 0;
	}
	else if (type == 0xd7) {
		//fixext 8
		return 0;
	}
	else if (type == 0xd8) {
		//fixext 16
		return 0;
	}
	else if (type == 0xc7) {
		//ext 8
		return 0;
	}
	else if (type == 0xc8) {
		//ext 16
		return 0;
	}
	else if (type == 0xc9) {
		//ext 32
		return 0;
	}
	return 0;
}


void EzyDataDecoder::onReadValue(entity::EzyValue* object) {
	if (mStack.empty()) {
		if (mDelegate)
			mDelegate->onReceivedMessage(object);
	}
	else{
		auto item = mStack.top();
        item->pushValue(object);
        if(item->validate()) {
            mStack.pop();
            auto value = item->toValue();
            EZY_SAFE_DELETE(item);
            this->onReadValue(value);
        }
	}
}

void EzyDataDecoder::onReadNil() {
    auto value = new entity::EzyValue();
	this->onReadValue(value);
}

void EzyDataDecoder::onReadBool(bool b) {
	auto value = new entity::EzyPrimitive();
	value->setBool(b);
	this->onReadValue(value);
}

void EzyDataDecoder::onReadInt(int64_t i64) {
	auto value = new entity::EzyPrimitive();
	value->setInt(i64);
	this->onReadValue(value);
}

void EzyDataDecoder::onReadUnsignedInt(uint64_t i64) {
	auto value = new entity::EzyPrimitive();
	value->setUInt(i64);
	this->onReadValue(value);
}

void EzyDataDecoder::onReadFloat(float f) {
	auto value = new entity::EzyPrimitive();
	value->setFloat(f);
	this->onReadValue(value);
}

void EzyDataDecoder::onReadDouble(double d) {
    auto value = new entity::EzyPrimitive();
	value->setDouble(d);
	this->onReadValue(value);
}

void EzyDataDecoder::onReadString(const char* str, uint32_t size) {
    auto value = new entity::EzyString();
	value->setData(str, size);
	this->onReadValue(value);
}

void EzyDataDecoder::onReadBin(const char* str, uint32_t size) {
	auto value = new entity::EzyString();
	value->setData(str, size);
	this->onReadValue(value);
}

void EzyDataDecoder::onReadMap(uint32_t size) {
	if (size == 0) {
		entity::EzyValue* value = new entity::EzyObject();
		this->onReadValue(value);
	}
	else{
        EzyArrayBuffer* arr = new EzyArrayBuffer(entity::EzyValueType::TypeDict, size * 2);
		mStack.push(arr);
	}	
}

void EzyDataDecoder::onReadArray(uint32_t size) {
	if (size == 0) {
        entity::EzyValue* value = new entity::EzyArray();
		this->onReadValue(value);
	}
	else{
        EzyArrayBuffer* arr = new EzyArrayBuffer(entity::EzyValueType::TypeArray, size);
		mStack.push(arr);
	}
}

EZY_NAMESPACE_END_WITH
