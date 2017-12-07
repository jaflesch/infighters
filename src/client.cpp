#include "client.h"

/* 
	Conecta ao server de matching
	Recebe informações de matching:
		ip do opponent
		se vai ser o admin (first a começar a partida e resposavel pela conexão)
*/

/*
// usado pela main de exemplo
DWORD WINAPI client(void* Socket) {
	client_info* player = client_searching();

	std::cout << "Matched with " << player->opponent_ip << std::endl << std::endl;

	return 1;
}
*/

client_info* client_searching() {
	int iResult;
	WSADATA wsaData;

	SOCKET ConnectSocket = INVALID_SOCKET;
	struct sockaddr_in clientService;

	int recvbuflen = DEFAULT_BUFLEN;
	char recvbuf[DEFAULT_BUFLEN] = "";

	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR) {
		wprintf(L"WSAStartup failed with error: %d\n", iResult);
		return NULL;
	}

	ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ConnectSocket == INVALID_SOCKET) {
		wprintf(L"socket failed with error: %ld\n", WSAGetLastError());
		WSACleanup();
		return NULL;
	}

	clientService.sin_family = AF_INET;
	inet_pton(AF_INET, IP_MACHING_SERVER, &(clientService.sin_addr.s_addr));			// ip matching server
	clientService.sin_port = htons(MATCHING_PORT);

	// Connect to server.
	do {
		iResult = connect(ConnectSocket, (SOCKADDR*)&clientService, sizeof(clientService));
	} while (iResult == SOCKET_ERROR);

	// Receive Opponents IP
	client_info * player = (client_info*)malloc(sizeof(client_info));

	iResult = 0;
	do {
		iResult += recv(ConnectSocket, player->opponent_ip, DEFAULT_BUFLEN, 0);
		if (iResult > 0)
			;//printf("Bytes received: %d\n", iResult);
		else if (iResult == 0)
			printf("Connection closed\n");
		else
			printf("recv failed: %d\n", WSAGetLastError());
	} while (iResult < DEFAULT_BUFLEN);

	char buffer[2];
	recv(ConnectSocket, buffer, sizeof(buffer), 0);
	if (!strcmp(buffer, "1"))
		player->first = true;
	else
		if (!strcmp(buffer, "0"))
			player->first = false;
		else
			std::cout << "Error receiving order info" << std::endl << std::endl;
	std::cout << "--- " << sizeof(buffer) << " ---" << std::endl;
	// close the socket
	iResult = closesocket(ConnectSocket);
	if (iResult == SOCKET_ERROR) {
		wprintf(L"close failed with error: %d\n", WSAGetLastError());
		WSACleanup();
		return NULL;
	}

	WSACleanup();
	return player;
}

SOCKET* connect(client_info * player) {
	int iResult;
	WSADATA wsaData;
	
	SOCKET * ConnectSocket = (SOCKET*)malloc(sizeof(SOCKET));
	*ConnectSocket = INVALID_SOCKET;
	struct sockaddr_in clientService;
	
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR) {
		wprintf(L"WSAStartup failed with error: %d\n", iResult);
		return NULL;
	}

	*ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (*ConnectSocket == INVALID_SOCKET) {
		wprintf(L"socket failed with error: %ld\n", WSAGetLastError());
		WSACleanup();
		return NULL;
	}

	clientService.sin_family = AF_INET;
	inet_pton(AF_INET, player->opponent_ip, &(clientService.sin_addr.s_addr));			// ip inimigo
	clientService.sin_port = htons(GAME_PORT);

	if (player->first) {
		SOCKET ListenSocket = INVALID_SOCKET;
		SOCKET * Invalid = (SOCKET*)malloc(sizeof(SOCKET));
		*Invalid = INVALID_SOCKET;
		
		struct addrinfo *result = NULL;
		struct addrinfo hints;
		
		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		hints.ai_flags = AI_PASSIVE;

		// Resolve the server address and port
		iResult = getaddrinfo(NULL, GAME_PORT_L, &hints, &result);
		if (iResult != 0) {
			printf("getaddrinfo failed with error: %d\n", iResult);
			WSACleanup();
			return Invalid;
		}

		// Create a SOCKET for connecting
		ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
		if (ListenSocket == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
			freeaddrinfo(result);
			WSACleanup();
			return Invalid;
		}

		// Setup the TCP listening socket
		iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			printf("bind failed with error: %d\n", WSAGetLastError());
			freeaddrinfo(result);
			closesocket(ListenSocket);
			WSACleanup();
			return Invalid;
		}

		freeaddrinfo(result);

		iResult = listen(ListenSocket, SOMAXCONN);
		if (iResult == SOCKET_ERROR) {
			printf("listen failed with error: %d\n", WSAGetLastError());
			closesocket(ListenSocket);
			WSACleanup();
			return Invalid;
		}
		
		bool player_connected = false;

		do {
			*ConnectSocket = accept(ListenSocket, NULL, NULL);
			if (*ConnectSocket == INVALID_SOCKET) {
				printf("accept failed with error: %d\n", WSAGetLastError());
				closesocket(ListenSocket);
				WSACleanup();
				return Invalid;
			}
			else {
				player_connected = true;
				printf("Player Connected\n\n");
			}
		} while (!player_connected);
	}
	else {
		// Connect to server.
		do {
			iResult = connect(*ConnectSocket, (SOCKADDR*)&clientService, sizeof(clientService));
			printf("Player Connected\n\n");
		} while (iResult == SOCKET_ERROR);				
	}

	return ConnectSocket;
}

int send_struct(client_info * player, SOCKET* ConnectSocket, teste* t) {
	int recvbuflen = DEFAULT_BUFLEN;
	char recvbuf[DEFAULT_BUFLEN] = "";

	int iResult;

	if (player->first) {
		send(*ConnectSocket, t->name, sizeof(t->name), 0);

		iResult = 0;
		do {
			iResult += recv(*ConnectSocket, t->name, sizeof(t->name), 0);
			if (iResult > 0)
				;//printf("Bytes received: %d\n", iResult);
			else if (iResult == 0) {
				printf("Connection closed\n");
				return 0;
			}
			else {
				printf("recv failed: %d\n", WSAGetLastError());
				return 0;
			}
		} while (iResult < sizeof(t->name));
	}
	else {
		iResult = 0;
		do {
			iResult += recv(*ConnectSocket, t->name, sizeof(t->name), 0);
			if (iResult > 0)
				;//printf("Bytes received: %d\n", iResult);
			else if (iResult == 0) {
				printf("Connection closed\n");
				return 0;
			}
			else {
				printf("recv failed: %d\n", WSAGetLastError());
				return 0;
			}
		} while (iResult < sizeof(t->name));

		send(*ConnectSocket, t->name, sizeof(t->name), 0);
	}
	printf("Recebido: %s\n\n", t->name);

	return 1;
}

/*
// main de exemplo
int main() {
	DWORD Thread2, Thread3;

	std::cout << "Client Searching" << std::endl;
	CreateThread(0, 0, client, NULL, 0, &Thread2);

	getchar();

	WSACleanup();
	return 0;
}
*/