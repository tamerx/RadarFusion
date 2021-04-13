#include "Server.h"


Server::Server()
{
	SOCKET ListenSocket = INVALID_SOCKET;
	SOCKET ClientSocket = INVALID_SOCKET;

	struct addrinfo *result = NULL;
}

int Server::init(int serverID)
{
	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		std::cout << ("WSAStartup failed with error: %d\n", iResult) << std::endl;
		return 1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	if (serverID == 1) {
		// Resolve the server address and port
		iResult = getaddrinfo(NULL, DEFAULT_RECEIVE_PORT_1, &hints, &result);
		if (iResult != 0) {
			printf("getaddrinfo failed with error: %d\n", iResult);
			WSACleanup();
			return 1;
		}
	}
	else if (serverID == 2) {
		// Resolve the server address and port
		iResult = getaddrinfo(NULL, DEFAULT_RECEIVE_PORT_2, &hints, &result);
		if (iResult != 0) {
			printf("getaddrinfo failed with error: %d\n", iResult);
			WSACleanup();
			return 1;
		}
	}
	else if (serverID == 3) {
		// Resolve the server address and port
		iResult = getaddrinfo(NULL, DEFAULT_SEND_PORT_3, &hints, &result);
		if (iResult != 0) {
			printf("getaddrinfo failed with error: %d\n", iResult);
			WSACleanup();
			return 1;
		}
	}

	else if (serverID == 4) {
		// Resolve the server address and port
		iResult = getaddrinfo(NULL, DEFAULT_SEND_PORT_4, &hints, &result);
		if (iResult != 0) {
			printf("getaddrinfo failed with error: %d\n", iResult);
			WSACleanup();
			return 1;
		}
	}

	// Create a SOCKET for connecting to server
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET) {
		printf("socket failed with error: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	// Setup the TCP listening socket
	iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	freeaddrinfo(result);


	

	iResult = listen(ListenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}
	return 0;
}

int Server::waitForClient()
{
	// Accept a client socket
	ClientSocket = accept(ListenSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET) {
		printf("accept failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	printf("Connection established with a client!\n");
	// No longer need server socket
	closesocket(ListenSocket);
	return 0;
}

int Server::readNBytesFromSocket(SOCKET socket, void * buf, int n)
{
	int offset = 0;
	char *cbuf = reinterpret_cast<char*>(buf);
	while (true) {
		int ret = recv(socket, cbuf + offset, n - offset, MSG_WAITALL);

		if (ret < 0) {
			if (errno != EINTR) {
				// Error occurred
				printf("Error Occurred\n");
				return -1;
			}
		}

		else if (ret == 0) {
			// No data available anymore
			if (offset == 0) return 0;
			else {
				printf("Unexpected end of stream\n");
				return -2;
			}
		}

		else if (offset + ret == n) {
			// All n bytes read
			return 1;
		}

		else {
			offset += ret;
		}
	}
}

int Server::sendNBytesFromSocket(SOCKET socket, char *  buf, int n)
{

	int offset = 0;
	char *cbuf = reinterpret_cast<char*>(buf);
	while (true) {
		int ret = send(socket, buf, n, 0);

		if (ret < 0) {
			if (errno != EINTR) {
				// Error occurred
				printf("Error Occurred\n");
				return -1;
			}
		}

		else if (ret == 0) {
			// No data available anymore
			if (offset == 0) return 0;
			else {
				printf("Unexpected end of stream\n");
				return -2;
			}
		}

		else if (offset + ret == n) {
			// All n bytes read
			return 1;
		}

		else {
			offset += ret;
			return 1;
		}
	}

	printf("Bytes Sent: %d\n", iResult);
}




int Server::terminate()
{
	// shutdown the connection since we're done
	iResult = shutdown(ClientSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(ClientSocket);
		WSACleanup();
		return 1;
	}

	// cleanup
	closesocket(ClientSocket);
	WSACleanup();

	return 0;
}