// Multiplayer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


#ifndef UNICODE
#define UNICODE
#endif

#define DEFAULT_BUFLEN 512
#define IP_MACHING_SERVER "127.0.0.1"
#define MATCHING_PORT 44443
#define MATCHING_PORT_L "44443"
#define DEFAULT_PORT 44444

//#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
//#else
//Linux
//#endif

#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <iostream>
#include <queue>
#include <string>
#include <stdio.h>
#include <tchar.h>

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

struct MyMessageType
{
	char newplayer[DEFAULT_BUFLEN];
};

std::queue<MyMessageType> messageQueue;
HANDLE mQSemaphore;
HANDLE SocketSemaphore;
HANDLE PrintSemaphore;


DWORD WINAPI ConectionManager(void* Socket)
{
	SOCKET ClientSocket = *((SOCKET*) Socket);
	ReleaseSemaphore(SocketSemaphore, 1, NULL);
	struct MyMessageType ip;
	bool matched = false;
	int id;
	char buffer[DEFAULT_BUFLEN];

	std::cout << "Player Searching" << std::endl << std::endl;

	socklen_t len;
	struct sockaddr_storage addr;
	char ipstr[INET6_ADDRSTRLEN];
	int port;

	len = sizeof addr;
	getpeername((SOCKET) ClientSocket, (struct sockaddr*)&addr, &len);

	// deal with both IPv4 and IPv6:
	if (addr.ss_family == AF_INET) {
		struct sockaddr_in *s = (struct sockaddr_in *)&addr;
		port = ntohs(s->sin_port);
		inet_ntop(AF_INET, &s->sin_addr, ipstr, sizeof ipstr);
	}
	else { // AF_INET6
		struct sockaddr_in6 *s = (struct sockaddr_in6 *)&addr;
		port = ntohs(s->sin6_port);
		inet_ntop(AF_INET6, &s->sin6_addr, ipstr, sizeof ipstr);
	}

	//std::cout << "Peer IP address: " << ipstr << std::endl << std::endl;

	strcpy_s(ip.newplayer, ipstr);

	// se o tamanho da fila eh dois -> espera esvaziar
	while (messageQueue.size() == 2);

	WaitForSingleObject(mQSemaphore, INFINITE);
	messageQueue.push(ip);
	id = messageQueue.size();

	// segundo da fila da match com o primeiro e deixa a fila intacta
	if (messageQueue.size() == 2) {
		matched = true;
		WaitForSingleObject(PrintSemaphore, INFINITE);
		std::cout << "Matching Realizado" << std::endl << std::endl;
		ReleaseSemaphore(PrintSemaphore, 1, NULL);
		strcpy_s(buffer, messageQueue.front().newplayer);
		send(ClientSocket, buffer, DEFAULT_BUFLEN, 0);
		send(ClientSocket, "0", sizeof("0"), 0);
	}
	ReleaseSemaphore(mQSemaphore, 1, NULL);

	// primeiro da fila da match com o segundo e retira os dois da fila
	if (matched == false) {
		while (messageQueue.size() != 2);
		WaitForSingleObject(mQSemaphore, INFINITE);

		messageQueue.pop();
		strcpy_s(buffer, messageQueue.front().newplayer);
		send(ClientSocket, buffer, DEFAULT_BUFLEN, 0);
		send(ClientSocket, "1", sizeof("1"), 0);
		messageQueue.pop();

		ReleaseSemaphore(mQSemaphore, 1, NULL);
	}

	return 0;
}

DWORD WINAPI matchmaking_server(LPVOID lpParam)
{
	using namespace std;
	WSADATA wsaData;
	int iResult;

	SOCKET ListenSocket = INVALID_SOCKET;
	SOCKET ClientSocket = INVALID_SOCKET;

	struct addrinfo *result = NULL;
	struct addrinfo hints;
	
	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;
	
	// Resolve the server address and port
	iResult = getaddrinfo(NULL, MATCHING_PORT_L, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return 1;
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
	
	while (ClientSocket = accept(ListenSocket, NULL, NULL)) {
		if (ClientSocket == INVALID_SOCKET) {
			printf("accept failed with error: %d\n", WSAGetLastError());
			closesocket(ListenSocket);
			WSACleanup();
			return 1;
		}
		else {	
			DWORD myThreadID;
			
			CreateThread(0, 0, ConectionManager, &ClientSocket, 0, &myThreadID);	
			WaitForSingleObject(SocketSemaphore, INFINITE);
		}
	}
	return 0;
}

int main() {
	DWORD Thread1;

	mQSemaphore = CreateSemaphore(
		NULL,           // default security attributes
		1,  // initial count
		1,  // maximum count
		NULL);          // unnamed semaphore
	if (mQSemaphore == NULL)
	{
		printf("CreateSemaphore error: %d\n", GetLastError());
		return 1;
	}

	SocketSemaphore = CreateSemaphore(NULL, 0, 1, NULL); 
	if (SocketSemaphore == NULL)
	{
		printf("CreateSemaphore error: %d\n", GetLastError());
		return 1;
	}
	PrintSemaphore = CreateSemaphore(NULL, 1, 1, NULL);

	std::cout << "Creating Server" << std::endl;
	CreateThread(0, 0, matchmaking_server, NULL, 0, &Thread1);

	getchar();

	WSACleanup();
	return 0;
}