#pragma comment(lib, "Ws2_32.lib")

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <WinSock2.h>

#define BUF_SIZE (1024)

using namespace std;

void error_handling(const char message[]);

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        puts("Usage : <IP> <Port>");
        return -1;
    }

    WSADATA wsaData;
    SOCKET sock;
    SOCKADDR_IN clntAddr;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        error_handling("WSAStartup() error");
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET)
    {
        error_handling("socket() error");
    }

    memset(&clntAddr, 0, sizeof(clntAddr));
    clntAddr.sin_family = AF_INET;
    clntAddr.sin_addr.s_addr = inet_addr(argv[1]);
    clntAddr.sin_port = htons(atoi(argv[2]));

    if (connect(sock, (SOCKADDR*)&clntAddr, sizeof(clntAddr)) == SOCKET_ERROR)
    {
        error_handling("connect() error");
    }

    char fileName[30];
    fputs("File Name : ", stdout);
    scanf("%s", fileName);
    send(sock, fileName, strlen(fileName), 0);

    FILE* fp = fopen("copy.jpg", "wb");
    if (fp == NULL)
    {
        error_handling("fopen() error");
    }

    char buffer[BUF_SIZE];
    int read_cnt;
    while ((read_cnt = recv(sock, buffer, BUF_SIZE, 0)) != 0)
    {
        if (read_cnt < BUF_SIZE)
        {
            fwrite(buffer, 1, read_cnt, fp);
            break;
        }
        fwrite(buffer, 1, BUF_SIZE, fp);
    }

    send(sock, "Thank you", 10, 0);

    fclose(fp);
    closesocket(sock);
    WSACleanup();

    return 0;
}

void error_handling(const char message[])
{
    puts(message);
    exit(1);
}