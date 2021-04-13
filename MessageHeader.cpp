#include <vector>

#include <winsock2.h>
#include <windows.h>

#include "MessageHeader.h"




MessageHeader::MessageHeader()
{
	this->messageID = 0;
	this->sentReceivedInfo = 0;
	this->messageLength = MESSAGE_HEADER_SIZE_IN_BYTES;

}

unsigned char MessageHeader::getSentReceivedInfo()
{
	return sentReceivedInfo;
}

void MessageHeader::setSentReceivedInfo(unsigned char sentReceivedInfo_)
{
	sentReceivedInfo = sentReceivedInfo_;
}

unsigned char MessageHeader::getMessageID()
{
	return messageID;
}

void MessageHeader::setMessageID(unsigned char messageID_)
{
	messageID = messageID_;
}

short MessageHeader::getMessageLength()
{
	return messageLength;
}

void MessageHeader::setMessageLength(short messageLength_)
{
	messageLength = messageLength_;
}




void MessageHeader::decode(unsigned char * data)
{

	ByteOperations byteOperations;
	int currentByteIndex = 0;

	// Sent/received info
	//sentReceivedInfo = (int)unsignedByteToInt(data, currentByteIndex);
	sentReceivedInfo = 0;
	currentByteIndex += sizeof(CHAR);

	// Message id
	//messageID = (int)unsignedByteToInt(data, currentByteIndex);
	messageID = 0;
	currentByteIndex += sizeof(CHAR);

	// Message length
	messageLength = (int)byteOperations.unsignedByteToShort(data, 2);
	currentByteIndex += sizeof(SHORT);
}


void MessageHeader::encode(int currentByteIndex,int isFused)
{

	ByteOperations byteOperations;
	int tempcurrentbyteIndex;

	tempcurrentbyteIndex = currentByteIndex;

	
	setSentReceivedInfo(0);
	//byteOperations.IntToByte(headerData, 0, this->sentReceivedInfo);
	headerData[0] = 0X21;
	currentByteIndex += sizeof(BYTE);

	if (isFused==1)
	{
		setMessageID(FUSE_STARTED_DROP);
	}
	else if (isFused==0){
		setMessageID(FUSE_STOP_DROP);
	}

	//byteOperations.IntToByte(headerData, 0, this->messageID);
	headerData[1] = this->messageID;
	currentByteIndex += sizeof(BYTE);

	setMessageLength(tempcurrentbyteIndex);
	byteOperations.shortToByte(headerData, 2, this->messageLength);
	currentByteIndex += sizeof(SHORT);
	std::cout << "Message Length :: " << getMessageLength() << std::endl;
	changeEndian(headerData, 2, 2);
	


	//return headerData;

}

int MessageHeader::computeMessageLength()
{
	return MessageHeader::MESSAGE_HEADER_SIZE_IN_BYTES;
}

void MessageHeader::changeEndian(unsigned char *data, int startIndex, int length) {
	byte tmp;

	for (int i = startIndex, j = startIndex + length - 1; i < j; i++, j--) {
		tmp = data[i];
		data[i] = data[j];
		data[j] = tmp;
	}
}



