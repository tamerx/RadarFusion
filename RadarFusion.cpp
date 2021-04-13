#include <iostream>
#include <thread>
#include <mutex>


#include "Message.h"
#include "Server.h"

HANDLE hSemaphore1;
HANDLE hSemaphore2;


#define _CRT_NONSTDC_NO_DEPRECATE
std::mutex mymtx;
std::mutex mymtx_radar;
std::mutex my_xmutex;
std::condition_variable is_not_empty_main;
std::condition_variable is_not_full_main;
int main(int argc, char *argv[])
{


	//bool IS_READ_DATA_FROM_FILE = argv[0];   // dosya veya mirsad 'tan veri alma seçne?i parametre olarak al?n?yor.

	//semaphore
	hSemaphore1 = CreateSemaphore(
		NULL,//Security Attributes
		0,//Initial State i.e.Non Signaled
		1,//No. of Resources
		NULL);//Semaphore Name

	hSemaphore2 = CreateSemaphore(
		NULL,//Security Attributes
		1,//Initial State i.e. Signaled
		1,//No. of Resources
		NULL);//Semaphore Name

	bool IS_READ_DATA_FROM_FILE = true;
	Radar radar;
	radar.mtx = &mymtx;
	radar.mtx_radar = &mymtx_radar;

	Message message(radar);
	message.xmutex = &my_xmutex;
	message.is_not_empty = &is_not_empty_main;
	message.is_not_full = &is_not_full_main;


	if (IS_READ_DATA_FROM_FILE)
	{
		//message.initializeOutputCSV();
		message.processMessageFromFile();

	}
	else if (!IS_READ_DATA_FROM_FILE)
	{
		message.initializeOutputCSV();
		message.initializeOutputCSV_mirsad();
		message.processMessageFromMirsad();

	}
	return 0;
}