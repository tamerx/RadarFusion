#ifndef MESSAGEHEADER
#define MESSAGEHEADER

#include "ByteOperations.h"

/**
 * Message header for radar messages consists of sent/received info,
 * message id and message length fields
 *
 */


class MessageHeader;


class MessageHeader {
	/**
	 * Field MESSAGE_HEADER_SIZE_IN_BYTES. (value is 4)
	 */


public:

#define MAX_MESSAGE_LENGTH 4096 // bytes	
#define FUSE_STARTED_DROP 0xFE
#define FUSE_STOP_DROP  0xFC
	static const int MESSAGE_HEADER_SIZE_IN_BYTES = 4;
	unsigned char headerData[MESSAGE_HEADER_SIZE_IN_BYTES];



private:
	/**
	 * Field sentReceivedInfo.
	 */
	unsigned char             sentReceivedInfo;
	/**
	 * Field messageID.
	 */
	unsigned char             messageID;
	/**
	 * Field messageLength.
	 */
	short             messageLength;

public:
	/**
	 * Default constructor
	 *
	 */
	MessageHeader();

	/**
	 *
	 * @return the sentReceivedInfo
	 */
	unsigned char getSentReceivedInfo();

	/**
	 * @param sentReceivedInfo
	 *            the sentReceivedInfo to set
	 */
	void setSentReceivedInfo(unsigned char sentReceivedInfo_);

	/**
	 *
	 * @return the messageID
	 */
	unsigned char getMessageID();

	/**
	 * @param messageID
	 *            the messageID to set
	 */
	void setMessageID(unsigned char messageID_);
	/**
	 *
	 * @return the messageLength
	 */
	short getMessageLength();

	/**
	 * @param messageLength
	 *            the messageLength to set
	 */
	void setMessageLength(short messageLength_);

	/**
	 * Encodes message header into the byte array
	 *
	 *
	 * @return encoded byte array
	 */
	void  encode(int currentByIndex,int isFused);

	/**
	 * @param data
	 *
	 *
	 * @return MessageHeader
	 */
	void decode(unsigned char * data);

	/**
	 * Computes message length value in bytes
	 *
	 *
	 * @return message length in bytes
	 */
	int computeMessageLength();

	void changeEndian(unsigned char *data, int startIndex, int length);

};

#endif