#pragma comment(lib, "Ws2_32.lib")

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#undef UNICODE
#undef _UNICODE

#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>

#define BUF_SIZE (30)

using namespace std;

void ErrorHandling(const char* message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hAcptSock, hRecvSock;

	SOCKADDR_IN recvAdr;
	SOCKADDR_IN sendAdr;
	int sendAdrSize, strLen;
	char buf[BUF_SIZE];
	int result;

	fd_set read, except, readCopy, exceptCopy;
	struct timeval timeout;

	if (argc != 2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	hAcptSock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&recvAdr, 0, sizeof(recvAdr));
	recvAdr.sin_family = AF_INET;
	recvAdr.sin_addr.s_addr = htonl(INADDR_ANY);
	recvAdr.sin_port = htons(atoi(argv[1]));

	if (bind(hAcptSock, (SOCKADDR*)&recvAdr, sizeof(recvAdr)) == SOCKET_ERROR)
		ErrorHandling("bind() error");
	if (listen(hAcptSock, 5) == SOCKET_ERROR)
		ErrorHandling("listen() error");

	sendAdrSize = sizeof(sendAdr);


	while (true)
	{
		hRecvSock = accept(hAcptSock, (SOCKADDR*)&sendAdr, &sendAdrSize);


		send(hRecvSock, "abcde", 5, 0);

		send(hRecvSock, "Hello, Last Message!\n", 21, 0);

		//shutdown(hRecvSock, 1);

		closesocket(hRecvSock);
	}

	closesocket(hAcptSock);
	WSACleanup();
	return 0;
}

void ErrorHandling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}