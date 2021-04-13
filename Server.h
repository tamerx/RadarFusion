#ifndef SERVER
#define SERVER

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <winsock2.h>
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
#include <random>
#include <sstream>
#include <random>
#include <string>
#include <windows.h>

#define DEFAULT_RECEIVE_PORT_1 "5300"
#define DEFAULT_RECEIVE_PORT_2 "5301"

#define DEFAULT_SEND_PORT_3 "5400"
#define DEFAULT_SEND_PORT_4 "5401"


#pragma comment(lib, "Ws2_32.lib")

class Server
{

public:
	Server();

	int init(int serverID);

	int waitForClient();

	/// Reads n bytes from the socket.
	int readNBytesFromSocket(SOCKET socket, void *buf, int n);
	int sendNBytesFromSocket(SOCKET socket, char *  buf, int n);

	int terminate();

	WSADATA wsaData;
	int iResult;

	SOCKET ListenSocket;
	SOCKET ClientSocket;

	struct addrinfo *result;
	struct addrinfo hints;

	int iSendResult;


	
};

#endif