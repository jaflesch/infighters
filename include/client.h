#pragma once

#define _WINSOCKAPI_ 
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <iostream>
#include <queue>
#include <string>
#include <stdio.h>
#include <tchar.h>


#define DEFAULT_BUFLEN 512
#define IP_MACHING_SERVER "127.0.0.1"			// ip do server q faz o matching
#define MATCHING_PORT 44443						// porta da conexao pro matching
#define MATCHING_PORT_L "44443"			
#define GAME_PORT 44444							// porta do jogo
#define GAME_PORT_L "44444"

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

/*
	Conecta ao server de matching
	Recebe informações de matching:
		ip do opponent
		se vai ser o admin (first a começar a partida e resposavel pela conexão)
*/

// struct retornada
struct client_info
{
	char opponent_ip[DEFAULT_BUFLEN];
	bool first;
};

struct teste
{
	char name[DEFAULT_BUFLEN];
};

client_info* client_searching();
SOCKET* connect(client_info * player);
int send_struct(client_info * player, SOCKET* ConnectSocket, teste* t);