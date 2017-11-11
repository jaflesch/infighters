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
	char *sendbuf = "Client: sending data test";
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
	iResult = connect(ConnectSocket, (SOCKADDR*)&clientService, sizeof(clientService));
	if (iResult == SOCKET_ERROR) {
		wprintf(L"connect failed with error: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return NULL;
	}

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