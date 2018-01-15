#define _WINSOCK_DEPRECATED_NO_WARNINGS
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

	/*
	u_long iMode = 1;

	iResult = ioctlsocket(ConnectSocket, FIONBIO, &iMode);
	if (iResult != NO_ERROR)
		printf("ioctlsocket failed with error: %ld\n", iResult);
	*/

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

	printf("ip do oponente: %s\n\n", player->opponent_ip);
	
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
		} while (iResult == SOCKET_ERROR);				
		printf("Player Connected\n\n");
	}
	
	return ConnectSocket;
}

char skill_to_char(Skill_ID skill) {
	char buffer;

	switch (skill) {
	case SKILL_NONE: buffer = '<'; break;
	case SKILL_FALSE_RUSH: buffer = 'a'; break;
	case SKILL_CONTRADICTION: buffer = 'b';	break;
	case SKILL_REQUIEM_ZERO:  buffer = 'c';	break;
	case SKILL_TRUTH_SLASH:   buffer = 'd';	break;
	case SKILL_TAUTOLOGY:     buffer = 'e';	break;
	case SKILL_AXIOM_ONE:     buffer = 'f';	break;
	case SKILL_BRUTE_FORCE:   buffer = 'g';	break;
	case SKILL_BUFFER_OVERFLOW:    buffer = 'h';	break;
	case SKILL_DDOS_ATTACK:   buffer = 'i';	break;
	case SKILL_PARTICLE_RENDERING: buffer = 'j';	break;
	case SKILL_DIFFUSE_REFLECTION: buffer = 'k';	break;
	case SKILL_DYNAMIC_FRUSTUM_ATTACK: buffer = 'l';	break;
	case SKILL_Q_PUNCH:       buffer = 'm';	break;
	case SKILL_PERCEPTRON:    buffer = 'n';	break;
	case SKILL_NEURAL_NETWORK: buffer = 'o';	break;
	case SKILL_PREEMPTION:    buffer = 'p';	break;
	case SKILL_MUTEX:        buffer = 'q';	break;
	case SKILL_THREAD_SCHEDULING: buffer = 'r';	break;
	case SKILL_PUMPING_UP:    buffer = 's';	break;
	case SKILL_AUTOMATA_SUMMON: buffer = 't';	break;
	case SKILL_TURING_MACHINE: buffer = 'u';	break;
	case SKILL_TMR:           buffer = 'v';	break;
	case SKILL_REDUNDANCY:    buffer = 'w';	break;
	case SKILL_ROLLBACK:      buffer = 'x';	break;
	case SKILL_ALT:           buffer = 'z';	break;
	case SKILL_CTRL:          buffer = '1';	break;
	case SKILL_DELETE:        buffer = '2';	break;
	case SKILL_BEST_BOUND_FIST: buffer = '3';	break;
	case SKILL_DUAL_SIMPLEX:  buffer = '4';	break;
	case SKILL_GRAPH_COLORING: buffer = '5';	break;
	case SKILL_SPRINT_BURST:  buffer = '6';	break;
	case SKILL_INHERITANCE:   buffer = '7';	break;
	case SKILL_OVERRIDE:      buffer = '8';	break;
	case SKILL_CLOCK_PULSE:   buffer = '9';	break;
	case SKILL_PIPELINE:      buffer = '0';	break;
	case SKILL_OVERCLOCK:     buffer = '=';	break;

		// Invulnerability skills
	case SKILL_BRANCH_DAMAGE: buffer = '+';	break;
	case SKILL_POLIMORPHISM: buffer = '-';	break;
	case SKILL_KNAPSACK_HIDEOUT: buffer = ')';	break;
	case SKILL_ESC: buffer = '(';	break;
	case SKILL_ROLLFORWARD: buffer = '*';	break;
	case SKILL_NON_DETERMINISM: buffer = '&';	break;
	case SKILL_FORK: buffer = '%';	break;
	case SKILL_HILL_CLIMBING: buffer = '$';	break;
	case SKILL_RASTERIZATION: buffer = '#';	break;
	case SKILL_ENCRYPTION: buffer = '@';	break;
	case SKILL_TRUE_ENDURANCE: buffer = '!';	break;
	case SKILL_VOID_BARRIER: buffer = '>';	break;
	default: buffer = '?';
	}
	return buffer;
}

Skill_ID char_to_skill(char buffer) {
	Skill_ID skill;

	switch (buffer) {
	case '<': skill = SKILL_NONE; break;
	case 'a': skill = SKILL_FALSE_RUSH; break;
	case 'b': skill = SKILL_CONTRADICTION;	break;
	case 'c': skill = SKILL_REQUIEM_ZERO;	break;
	case 'd': skill = SKILL_TRUTH_SLASH;	break;
	case 'e': skill = SKILL_TAUTOLOGY;	break;
	case 'f': skill = SKILL_AXIOM_ONE;	break;
	case 'g': skill = SKILL_BRUTE_FORCE;	break;
	case 'h': skill = SKILL_BUFFER_OVERFLOW;	break;
	case 'i': skill = SKILL_DDOS_ATTACK;	break;
	case 'j': skill = SKILL_PARTICLE_RENDERING;	break;
	case 'k': skill = SKILL_DIFFUSE_REFLECTION;	break;
	case 'l': skill = SKILL_DYNAMIC_FRUSTUM_ATTACK;	break;
	case 'm': skill = SKILL_Q_PUNCH;	break;
	case 'n': skill = SKILL_PERCEPTRON;	break;
	case 'o': skill = SKILL_NEURAL_NETWORK;	break;
	case 'p': skill = SKILL_PREEMPTION;	break;
	case 'q': skill = SKILL_MUTEX;	break;
	case 'r': skill = SKILL_THREAD_SCHEDULING;	break;
	case 's': skill = SKILL_PUMPING_UP;	break;
	case 't': skill = SKILL_AUTOMATA_SUMMON;	break;
	case 'u': skill = SKILL_TURING_MACHINE;	break;
	case 'v': skill = SKILL_TMR;	break;
	case 'w': skill = SKILL_REDUNDANCY;	break;
	case 'x': skill = SKILL_ROLLBACK;	break;
	case 'z': skill = SKILL_ALT;	break;
	case '1': skill = SKILL_CTRL;	break;
	case '2': skill = SKILL_DELETE;	break;
	case '3': skill = SKILL_BEST_BOUND_FIST;	break;
	case '4': skill = SKILL_DUAL_SIMPLEX;	break;
	case '5': skill = SKILL_GRAPH_COLORING;	break;
	case '6': skill = SKILL_SPRINT_BURST;	break;
	case '7': skill = SKILL_INHERITANCE;	break;
	case '8': skill = SKILL_OVERRIDE;	break;
	case '9': skill = SKILL_CLOCK_PULSE;	break;
	case '0': skill = SKILL_PIPELINE;	break;
	case '=': skill = SKILL_OVERCLOCK;	break;

		// Invulnerability skills
	case '+': skill = SKILL_BRANCH_DAMAGE;	break;
	case '-': skill = SKILL_POLIMORPHISM;	break;
	case ')': skill = SKILL_KNAPSACK_HIDEOUT;	break;
	case '(': skill = SKILL_ESC;	break;
	case '*': skill = SKILL_ROLLFORWARD;	break;
	case '&': skill = SKILL_NON_DETERMINISM;	break;
	case '%': skill = SKILL_FORK;	break;
	case '$': skill = SKILL_HILL_CLIMBING;	break;
	case '#': skill = SKILL_RASTERIZATION;	break;
	case '@': skill = SKILL_ENCRYPTION;	break;
	case '!': skill = SKILL_TRUE_ENDURANCE;	break;
	case '>': skill = SKILL_VOID_BARRIER;	break;
	}
	return skill;
}

char charID_to_char(Character_ID character) {
	char buffer;

	switch (character) {
	case CHAR_NONE: buffer = 'a'; break;
	case CHAR_ZERO: buffer = 'b';	break;
	case CHAR_ONE: buffer = 'c';	break;
	case CHAR_SERIAL_KEYLLER: buffer = 'd';	break;
	case CHAR_RAY_TRACEY: buffer = 'e';	break;
	case CHAR_A_STAR: buffer = 'f';	break;
	case CHAR_DEADLOCK: buffer = 'g';	break;
	case CHAR_NORMA: buffer = 'h';	break;
	case CHAR_HAZARD: buffer = 'i';	break;
	case CHAR_QWERTY: buffer = 'j';	break;
	case CHAR_BIG_O: buffer = 'k';	break;
	case CHAR_NEW: buffer = 'l';	break;
	case CHAR_CLOCKBOY: buffer = 'm';	break;
	}
	return buffer;
}

Character_ID char_to_charID(char buffer) {
	Character_ID character = (Character_ID)-1;

	switch (buffer) {
	case 'a': character = CHAR_NONE; break;
	case 'b': character = CHAR_ZERO;	break;
	case 'c': character = CHAR_ONE;	break;
	case 'd': character = CHAR_SERIAL_KEYLLER;	break;
	case 'e': character = CHAR_RAY_TRACEY;	break;
	case 'f': character = CHAR_A_STAR;	break;
	case 'g': character = CHAR_DEADLOCK;	break;
	case 'h': character = CHAR_NORMA;	break;
	case 'i': character = CHAR_HAZARD;	break;
	case 'j': character = CHAR_QWERTY;	break;
	case 'k': character = CHAR_BIG_O;	break;
	case 'l': character = CHAR_NEW;	break;
	case 'm': character = CHAR_CLOCKBOY;	break;
	default: character = CHAR_NONE; break; // @TODO crash here why?
	}
	return character;
}

char num_to_char(int num) {
	char c = 0;

	switch (num) {
	case 0:
		c = '0';
		break;
	case 1:
		c = '1';
		break;
	case 2:
		c = '2';
		break;
	case -1:
		c = '-';
		break;
	default:
		printf("Erro no num_to_char: %d\n", num);
		break;
	}

	return c;
}

int char_to_num(char c) {
	int num = 0;

	switch (c) {
	case '0':
		num = 0;
		break;
	case '1':
		num = 1;
		break;
	case '2':
		num = 2;
		break;
	case '-':
		num = -1;
		break;
	default:
		printf("Erro no char_to_num: %c\n", c);
		break;
	}

	return num;
}

int exchange_char_selection(SOCKET* ConnectSocket, client_info * player, Char_Selection_State * characters){
	char recvbuf[4];
	int iResult;

	if (player->first) {
		recvbuf[0] = charID_to_char((Character_ID)characters->selections[0]);
		recvbuf[1] = charID_to_char((Character_ID)characters->selections[1]);
		recvbuf[2] = charID_to_char((Character_ID)characters->selections[2]);
		
		send(*ConnectSocket, recvbuf, sizeof(recvbuf), 0);
		
		iResult = 0;
		do {
			iResult += recv(*ConnectSocket, recvbuf, sizeof(recvbuf), 0);
			if (iResult > 0)
				;//printf("Bytes received: %d\n", iResult);
			else if (iResult == 0) {
				printf("Connection closed\n");
				return 0;
			}
			else {
				//printf("recv failed: %d\n", WSAGetLastError());
				return 0;
			}
		} while (iResult != sizeof(recvbuf));
		characters->enemy_selections[0] = char_to_charID(recvbuf[0]);
		characters->enemy_selections[1] = char_to_charID(recvbuf[1]);
		characters->enemy_selections[2] = char_to_charID(recvbuf[2]);
	}
	else {		
		iResult = 0;
		do {
			iResult += recv(*ConnectSocket, recvbuf, sizeof(recvbuf), 0);
			if (iResult > 0)
				;//printf("Bytes received: %d\n", iResult);
			else if (iResult == 0) {
				printf("Connection closed\n");
				return 0;
			}
			else {
				//printf("recv failed: %d\n", WSAGetLastError());
				return 0;
			}
		} while (iResult != sizeof(recvbuf));
		characters->enemy_selections[0] = char_to_charID(recvbuf[0]);
		characters->enemy_selections[1] = char_to_charID(recvbuf[1]);
		characters->enemy_selections[2] = char_to_charID(recvbuf[2]);

		recvbuf[0] = charID_to_char((Character_ID)characters->selections[0]);
		recvbuf[1] = charID_to_char((Character_ID)characters->selections[1]);
		recvbuf[2] = charID_to_char((Character_ID)characters->selections[2]);
		recvbuf[3] = '\0';

		send(*ConnectSocket, recvbuf, sizeof(recvbuf), 0);
	}
	//printf("%d %d %d %d %d %d\n", characters->selections[0], characters->selections[1], characters->selections[2], characters->enemy_selections[0], characters->enemy_selections[1], characters->enemy_selections[2]);

	u_long iMode = 1;

	iResult = ioctlsocket(*ConnectSocket, FIONBIO, &iMode);
	if (iResult != NO_ERROR)
		printf("ioctlsocket failed with error: %ld\n", iResult);

	return 1;
}

int send_struct(SOCKET* ConnectSocket, Target target) {
	//struct teste t;
	
	char buffer = skill_to_char(target.skill_used);		
		
	send(*ConnectSocket, &buffer, sizeof(buffer), 0);
	printf("Enviado (skill used): %c\n\n", buffer);
	
	buffer = charID_to_char(target.attacking_character);
	send(*ConnectSocket, &buffer, sizeof(buffer), 0);
	printf("Enviado (attacking character): %c\n\n", buffer);
	
	
	for (int k = 0; k < NUM_ENEMIES; ++k) {
		buffer = num_to_char(target.enemy_target_index[k]);
		send(*ConnectSocket, &buffer, sizeof(buffer), 0);
		printf("Enviado (enemy target index %d): %d\n\n", k, target.enemy_target_index[k]);
	}

	for (int k = 0; k < NUM_ALLIES; ++k) {
		buffer = num_to_char(target.ally_target_index[k]);
		send(*ConnectSocket, &buffer, sizeof(buffer), 0);
		printf("Enviado (ally target index %d): %d\n\n", k, target.ally_target_index[k]);
	}

	return 1;
}

int receive_struct(SOCKET* ConnectSocket, Target * targets) {
	int recvbuflen = DEFAULT_BUFLEN;
	char recvbuf[DEFAULT_BUFLEN] = "";

	//struct teste t;
	int iResult;
			
	for (int i = 0; i < NUM_ALLIES; ++i) {
		iResult = 0;
		do {
			iResult += recv(*ConnectSocket, recvbuf, sizeof(char), 0);
			if (iResult > 0)
				;//printf("Bytes received: %d\n", iResult);
			else if (iResult == 0) {
				printf("Connection closed\n");
				return 0;
			}
			else {
				//printf("recv failed: %d\n", WSAGetLastError());
				return 0;
			}
		} while (iResult != sizeof(char));
		printf("Recebido (skill used): %c\n\n", recvbuf[0]);
		targets[i].skill_used = char_to_skill(recvbuf[0]);
		
		iResult = 0;
		do {
			iResult += recv(*ConnectSocket, recvbuf, sizeof(char), 0);
			if (iResult > 0)
				;//printf("Bytes received: %d\n", iResult);
			else if (iResult == 0) {
				printf("Connection closed\n");
				return 0;
			}
			else {
				//printf("recv failed: %d\n", WSAGetLastError());
				return 0;
			}
		} while (iResult != sizeof(char));
		printf("Recebido (attacking character): %c\n\n", recvbuf[0]);
		targets[i].attacking_character = char_to_charID(recvbuf[0]);
						
		for (int k = 0; k < NUM_ENEMIES; ++k) {
			iResult = 0;
			do {
				iResult += recv(*ConnectSocket, recvbuf, sizeof(char), 0);
				if (iResult > 0)
					;//printf("Bytes received: %d\n", iResult);
				else if (iResult == 0) {
					printf("Connection closed\n");
					return 0;
				}
				else {
					//printf("recv failed: %d\n", WSAGetLastError());
					return 0;
				}
			} while (iResult != sizeof(char));
			targets[i].enemy_target_index[k] = char_to_num(recvbuf[0]);
			printf("Recebido (enemy target index %d): %c\n\n", k, targets[i].enemy_target_index[k]);
		}

		for (int k = 0; k < NUM_ALLIES; ++k) {
			iResult = 0;
			do {
				iResult += recv(*ConnectSocket, recvbuf, sizeof(char), 0);
				if (iResult > 0)
					;//printf("Bytes received: %d\n", iResult);
				else if (iResult == 0) {
					printf("Connection closed\n");
					return 0;
				}
				else {
					//printf("recv failed: %d\n", WSAGetLastError());
					return 0;
				}
			} while (iResult != sizeof(char));

			targets[i].ally_target_index[k] = char_to_num(recvbuf[0]);
			printf("Recebido (ally target index %d): %c\n\n", k, targets[i].ally_target_index[k]);
		}
	}
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