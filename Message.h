#ifndef MESSAGE
#define MESSAGE

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
#include <map>
#include <ctime>
#include <fstream>
#include <sstream>
#include <limits>
#include <atomic>
#include <stack>

#include<Windows.h>
#include<iostream> //Binary Semaphore



#include "Server.h"
#include "Radar.h"
#include "TrackInstance.h"
#include "MessageHeader.h"





class Message {

	public:


		#define MAX_MESSAGE_LENGTH 4024 // bytes

		MessageHeader messageHeader;
		TrackInstance trackInstance;
		Radar radar;
		bool fuseFlag = false;

		int cursor;
		std::vector<Message> messages;	// accumulating all Messages in vector messages. 
		int radarsVectorIsBeingUsed = 0;
		//std::vector<std::string> fusedArray;
		std::map<std::string, int> fusedArray;
		unsigned char messageData[MessageHeader::MESSAGE_HEADER_SIZE_IN_BYTES + MAX_MESSAGE_LENGTH];
		Message();
		Message(Radar radar);
		Message(MessageHeader messageHeader_);
		MessageHeader getMessageHeader();





		// *********************************************************
		const int num_producers = 5;
		const int num_consumers = 10;
		const int producer_delay_to_produce = 1000;   // in miliseconds
		const int consumer_delay_to_consume = 30;     // in miliseconds

		const int consumer_max_wait_time = 200;       // in miliseconds - max time that a consumer can wait for a product to be produced.

		const int max_production = 10;                // When producers has produced this quantity they will stop to produce
		const int max_products = 10;                  // Maximum number of products that can be stored

		//
		//      Variables
		//
		std::atomic<int> *num_producers_working = 0;      // When there's no producer working the consumers will stop, and the program will stop.
		std::mutex *xmutex;                               // Our mutex, without this mutex our program will cry

		std::condition_variable *is_not_full;             // to indicate that our stack is not full between the thread operations
		std::condition_variable *is_not_empty;            // to indicate that our stack is not empty between the thread operations
		// ***********************************************************************



		void setMessageHeader(MessageHeader messageHeader_);
		void encode(int &currentByIndex);
		void decode(unsigned char * data);
		int computeMessageLength();
		void setTrackParameters(int id, int range, double azimuth, double velocity, double directionAngle, double radialVelocity,
			int status, int classificationStatus, double snr, double latitude, double longitude, long time);
		void setRadarParameters(unsigned char ip[4], int  radar_id, double latitude, double longitude, double northYaw,
			double centerAngle, double halfwidth, double minRadii, double maxRadii);


		void processNewMessage(Message &newMessage);
		void processMessageFromFile();
		void processMessageFromFileThreadFunction();
		void processFusedMessageFromFileThreadFunction();
		void readData(int startFromNthLine,std::vector<Message> &messages);
		void sendFuseDataToMirsad(int startFromNthLine, Server &server_gonder_1, Server &server_gonder_2);
		void printEverything(std::vector<Radar> radars);
		void initializeOutputCSV();
		void initializeOutputCSV_mirsad();
		void appendRawDataToCSV(Message &message, int radarId);
		void appendRawDataToCSV_mirsad(Message &message, int radarId, int basladi_bitti);
		static  void appendFuseDataToCSV(TrackInstance &track, long newId, int trackMatchCount, std::string myUniqueID);
		void processMessageFromMirsad();
		int server_thread_function(int parameter);
		std::thread server_thread(int *paramter);
		std::thread proces_message_thread();
		std::thread proces_fuse_message_thread();
		void debugArray(const unsigned char* data, size_t len);
		std::vector<std::string> split(const std::string &s, char delim);
};




#endif