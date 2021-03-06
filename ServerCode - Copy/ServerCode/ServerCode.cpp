// ServerCode.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <iostream>
#include <typeinfo>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

using namespace std;

int __cdecl main(void)
{
	WSADATA wsaData;
	int iResult;

	SOCKET ListenSocket = INVALID_SOCKET;
	SOCKET ClientSocket = INVALID_SOCKET;

	struct addrinfo *result = NULL;
	struct addrinfo hints;

	int iSendResult;
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;

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
	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
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

	// Accept a client socket
	ClientSocket = accept(ListenSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET) {
		printf("accept failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	// No longer need server socket
	//TODO renable this closesocket(ListenSocket);
	char *stringSearch;

	// Receive until the peer shuts down the connection
	while (recvbuf != "shutdown")
	{
		for (int i = 0; i < sizeof(recvbuf); i++)
		{
			recvbuf[i] = NULL;
		}

		iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0) {
			printf("Bytes received: %d\n", iResult);
			for (int i = 0; i < sizeof(recvbuf); i++)
			{

				switch (recvbuf[i])
				{
				case 'a': cout << "a";
					break;
				case 'b': cout << "b";
					break;
				case 'c': cout << "c";
					break;
				case 'd': cout << "d";
					break;
				case 'e': cout << "e";
					break;
				case 'f': cout << "f";
					break;
				case 'g': cout << "g";
					break;
				case 'h': cout << "h";
					break;
				case 'i': cout << "i";
					break;
				case 'j': cout << "j";
					break;
				case 'k': cout << "k";
					break;
				case 'l': cout << "l";
					break;
				case 'm': cout << "m";
					break;
				case 'n': cout << "n";
					break;
				case 'o': cout << "o";
					break;
				case 'p': cout << "p";
					break;
				case 'q': cout << "q";
					break;
				case 'r': cout << "r";
					break;
				case 's': cout << "s";
					break;
				case 't': cout << "t";
					break;
				case 'u': cout << "u";
					break;
				case 'v': cout << "v";
					break;
				case 'w': cout << "w";
					break;
				case 'x': cout << "x";
					break;
				case 'y': cout << "y";
					break;
				case 'z': cout << "z";
					break;
				case ' ': cout << " ";
					break;
				case '_': cout << " ";
				}

			}
			printf("\n");
			// Echo the buffer back to the sender
			iSendResult = send(ClientSocket, recvbuf, iResult, 0);
			if (iSendResult == SOCKET_ERROR) {
				printf("send failed with error: %d\n", WSAGetLastError());
				closesocket(ClientSocket);
				WSACleanup();
				return 1;
			}
			printf("Bytes sent: %d\n", iSendResult);
		}
		else if (iResult != 0 and iResult < 1){
			printf("recv failed with error: %d\n", WSAGetLastError());
			closesocket(ClientSocket);
			WSACleanup();
			return 1;
		}

		stringSearch = strstr(recvbuf, "shutdown");
		if (stringSearch != NULL)
		{
			break;
		}
		
	}

	iSendResult = send(ClientSocket, "shutdown", iResult, 0);

	printf("Connection closing...\n");

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


