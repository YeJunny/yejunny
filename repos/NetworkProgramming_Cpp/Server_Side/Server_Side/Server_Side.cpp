#pragma comment(lib, "Ws2_32.lib")

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#undef UNICODE
#undef _UNICODE

#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>

#define BUF_SIZE (1024)

using namespace std;

void error_handling(const char message[]);

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        puts("Usage : <port>");
        return -1;
    }

    wchar_t name[] = L"Hello, World!";
    char name2[] = "Hello, World!";
    cout << name << " " << name2 << endl;
    cout << wcslen(name) << " " << strlen(name2) << endl;

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData))
    {
        error_handling("WSAStartup() error");
    }

    SOCKET servSocket;
    servSocket = socket(PF_INET, SOCK_STREAM, 0);
    if (servSocket == INVALID_SOCKET)
    {
        error_handling("socket() error");
    }

    sockaddr_in servAddr;
    memset(&servAddr, 0, sizeof(sockaddr_in));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(atoi(argv[1]));
    if (bind(servSocket, (sockaddr*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
    {
        error_handling("bind() error");
    }

    if (listen(servSocket, 5) == SOCKET_ERROR)
    {
        error_handling("listen() error");
    }
    
    sockaddr_in clntAddr;
    int clntAddrSize = sizeof(clntAddr);
    SOCKET clntSocket = accept(servSocket, (sockaddr*)&clntAddr, &clntAddrSize);
    if (clntSocket == SOCKET_ERROR)
    {
        error_handling("accept() error");
    }
    else
    {
        cout << "Connected client\n";
    }

    char fileName[30];
    int fileNameLen = recv(clntSocket, fileName, 30, 0);
    fileName[fileNameLen] = 0;

    FILE* fp = fopen(fileName, "rb");
    if (fp == NULL)
    {
        error_handling("fopen() error");
    }

    char message[BUF_SIZE];
    int read_len;
    while ((read_len = fread(message, 1, BUF_SIZE, fp)) != 0)
    {
        if (read_len < BUF_SIZE)
        {
            send(clntSocket, message, read_len, 0);
            break;
        }

        send(clntSocket, message, BUF_SIZE, 0);
    }
    puts("Send Complete!");

    shutdown(clntSocket, SD_SEND);
    recv(clntSocket, message, BUF_SIZE, 0);
    printf("Message from Client : %s \n", message);

    fclose(fp);
    closesocket(clntSocket);
    closesocket(servSocket);

    WSACleanup();

    return 0;
}

void error_handling(const char message[])
{
    puts(message);
    exit(1);
}