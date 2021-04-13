#pragma once
//#include <stdio.h>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <fstream>
#include <sstream>
#include <limits>
#include <thread>


class ByteOperations {

public:
	typedef unsigned int uint;
	unsigned char BYTE_OPERATIONS_STATIC_BYTE[4];
	short unsignedByteToShort(unsigned char *data, int start_index);
	int unsignedByteToInt(unsigned char *data, int start_index);
	uint64_t unsignedByteToLong(unsigned char *data, int start_index);
	unsigned char * intToUnsignedByte(int n);
	unsigned char * longToUnsignedByte(long longInt);
	unsigned char * doubleToUnsignedByte(double longInt);
	std::vector<unsigned char> intToBytes(int paramInt);
	void IntToByte(unsigned char* buf, int index, int value);
	void FloatToByte(unsigned char* buf, int index, float value);
	void LongToByte(unsigned char* buf, int index, long value);
	void DoubleToByte(unsigned char* buf, int index, double value);
	void shortToByte(unsigned char *buf, int index, uint i);

};
