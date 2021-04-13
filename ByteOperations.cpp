#include "ByteOperations.h"
short ByteOperations::unsignedByteToShort(unsigned char *data, int start_index) {
	return (data[start_index] << 8) | data[start_index + 1];
}
int ByteOperations::unsignedByteToInt(unsigned char *data, int start_index) {
	//return ((data[start_index] << 24) | (data[start_index+1] << 16) | (data[start_index+2] << 8) | data[start_index+3]);
	return ((unsignedByteToShort(data, start_index) << 16) |
		unsignedByteToShort(data, start_index + 2));
}
uint64_t ByteOperations::unsignedByteToLong(unsigned char *data, int start_index) {
	uint64_t value = static_cast<uint64_t> (*(data + start_index + 0)) |
						static_cast<uint64_t> (*(data + start_index + 1)) << 8 |
						static_cast<uint64_t> (*(data + start_index + 2)) << 16 |
						static_cast<uint64_t> (*(data + start_index + 3)) << 24 |
						static_cast<uint64_t> (*(data + start_index + 4)) << 32 |
						static_cast<uint64_t> (*(data + start_index + 5)) << 40 |
						static_cast<uint64_t> (*(data + start_index + 6)) << 48 |
						static_cast<uint64_t> (*(data + start_index + 7)) << 56;
	//return ((unsignedByteToInt(data, start_index) << 32) |
	//        unsignedByteToInt(data, start_index + 4));
	return value;
}
unsigned char *ByteOperations::intToUnsignedByte(int n) {
	unsigned char *bytes = BYTE_OPERATIONS_STATIC_BYTE;
	bytes[0] = (n >> 24) & 0xFF;
	bytes[1] = (n >> 16) & 0xFF;
	bytes[2] = (n >> 8) & 0xFF;
	bytes[3] = n & 0xFF;
	return bytes;
}
unsigned char *ByteOperations::longToUnsignedByte(long longInt) {
	unsigned char *bytes = BYTE_OPERATIONS_STATIC_BYTE;
	// convert from an unsigned long int to a 4-byte array
	bytes[0] = (int)((longInt >> 24) & 0xFF);
	bytes[1] = (int)((longInt >> 16) & 0xFF);
	bytes[2] = (int)((longInt >> 8) & 0XFF);
	bytes[3] = (int)((longInt & 0XFF));
	return bytes;
}
std::vector < unsigned char > ByteOperations::intToBytes(int paramInt) {
	std::vector < unsigned char > arrayOfByte(4);
	for (int i = 0; i < 4; i++)
		arrayOfByte[3 - i] = (paramInt >> (i * 8));
	return arrayOfByte;
}
unsigned char *ByteOperations::doubleToUnsignedByte(double doubleValue) {
	unsigned char bytes[sizeof(double)];
	memcpy(bytes, &doubleValue, sizeof(double));
	return bytes;
}
void ByteOperations::IntToByte(unsigned char *buf, int index, int value) {
	buf[index] = (char)(value & 0xff);
	buf[index + 1] = (char)((value & 0xff00) >> 8);
	buf[index + 2] = (char)((value & 0xff0000) >> 16);
	buf[index + 3] = (char)((value & 0xff000000) >> 24);
}
void ByteOperations::shortToByte(unsigned char *buf, int index,uint i)
{
	//memset(buf, 0, sizeof(unsigned char) * 2);
	buf[0+ index] = (unsigned char)(0xff & i);
	buf[1+ index] = (unsigned char)((0xff00 & i) >> 8);
}


void ByteOperations::FloatToByte(unsigned char *buf, int index, float value) {
	char *ivalue = (char*)&value;
	buf[index] = ivalue[0];
	buf[index + 1] = ivalue[1];
	buf[index + 2] = ivalue[2];
	buf[index + 3] = ivalue[3];
}
void ByteOperations::LongToByte(unsigned char *buf, int index, long value) {
	int int1 = (int)(value & 0x7FFFFFFF);
	int int2 = (int)((value - int1) >> 31);
	IntToByte(buf, index, int2);
	IntToByte(buf, index + 4, int1);
}
void ByteOperations::DoubleToByte(unsigned char *buf, int index, double value) {
	unsigned char *data = (unsigned char *)&value;
	buf[index + 0] = data[0];
	buf[index + 1] = data[1];
	buf[index + 2] = data[2];
	buf[index + 3] = data[3];
	buf[index + 4] = data[4];
	buf[index + 5] = data[5];
	buf[index + 6] = data[6];
	buf[index + 7] = data[7];
	buf + 8;
}