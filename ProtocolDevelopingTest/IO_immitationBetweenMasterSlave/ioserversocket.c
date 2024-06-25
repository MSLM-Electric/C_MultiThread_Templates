#ifndef UNICODE
#define UNICODE
#endif

#define WIN32_LEAN_AND_MEAN

#include <Ws2tcpip.h>
#include <stdio.h>
#include "iosocket.h"

// Link with ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT 27015//502

Timerwp_t ioserverResponsePeriod;
Timerwp_t ioserverRecvPeriod;

DWORD WINAPI ioserversock_task(LPVOID lpParam)
{
    //----------------------
    // Declare and initialize variables.
    int iResult;
    WSADATA wsaData;

    ListenSocket = INVALID_SOCKET;
    struct sockaddr_in serverService;

    int recvbuflen = DEFAULT_BUFLEN;
    char* sendbuf = "Server: sending data test";
    char recvbuf[DEFAULT_BUFLEN] = "";

    //----------------------
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != NO_ERROR) {
        wprintf(L"WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    //----------------------
    // Create a SOCKET for connecting to server
    ListenSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); //SOCK_DGRAM, IPPROTO_UDP - in case of udp not required to listen
    if (ListenSocket == INVALID_SOCKET) {
        wprintf(L"socket failed with error: %ld\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    //----------------------
    // The sockaddr_in structure specifies the address family,
    // IP address, and port of the server to be connected to.
    serverService.sin_family = AF_INET;
    serverService.sin_addr.s_addr = inet_addr("192.168.88.250");//inet_addr("127.0.0.1");
    serverService.sin_port = htons(DEFAULT_PORT);

    //----------------------
    // Connect to server.
    iResult = bind(ListenSocket, (SOCKADDR*)&serverService, sizeof(serverService));
    if (iResult == SOCKET_ERROR) {
        wprintf(L"connect failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    InitTimerWP(&ioserverResponsePeriod, (tickptr_fn*)GetTickCount);
    LaunchTimerWP((U32_ms)1000, &ioserverResponsePeriod);
    InitTimerWP(&ioserverRecvPeriod, (tickptr_fn*)GetTickCount);
    LaunchTimerWP((U32_ms)1000, &ioserverRecvPeriod);

    SOCKADDR_IN remoteNodeAddr;
    int remoteNodeAddrSize = sizeof(remoteNodeAddr);
    remoteNodeAddrSize = sizeof(serverService);
    char buffer[300];
    int res = 0;
    int siz = sizeof(buffer);
    //sprintf(buffer, "server respond");

    for (;;)
    {
        //RestartTimerWP(&ioserverResponsePeriod);
        //while (NOT IsTimerWPRinging(&ioserverResponsePeriod));
        RestartTimerWP(&ioserverRecvPeriod);
        
        do {
            res = recvfrom(ListenSocket, buffer, sizeof(buffer), 0, &serverService, &remoteNodeAddrSize); //?strlen(buffer) is dangerous when receiving
            siz = strlen(buffer);
            if (IsTimerWPRinging(&ioserverRecvPeriod)) {
                printf("server not fully recved\n");
                res = -1;
                break;
            }
                
        } while (res > 0);
        //----------------------
        // Send an initial buffer
        if (res != -1) {
            buffer[res] = 0;
            printf("server recved: %s\n", buffer);
            sprintf(buffer, "server responds!\n\0");
            res = sendto(ListenSocket, buffer, strlen(buffer), 0, &serverService, &remoteNodeAddrSize);
        }
        else {
            continue;
        }
        //if (res == SOCKET_ERROR) {
        //    wprintf(L"send failed with error: %d\n", WSAGetLastError());
        //    closesocket(ListenSocket);
        //    WSACleanup();
        //    return 1;  //?! Exits in here!
        //}

        printf("Bytes Sent: %d\n", res);   
    }
    // close the socket
    iResult = closesocket(ListenSocket);
    if (iResult == SOCKET_ERROR) {
        wprintf(L"close failed with error: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    WSACleanup();
    return 0;
}

int CreateServerSocket(void)
{
    //----------------------
    // Declare and initialize variables.
    int iResult;
    WSADATA wsaData;

    SOCKET ListenSocket = INVALID_SOCKET;
    struct sockaddr_in serverService;

    int recvbuflen = DEFAULT_BUFLEN;
    char* sendbuf = "Server: sending data test";
    char recvbuf[DEFAULT_BUFLEN] = "";

    //----------------------
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != NO_ERROR) {
        wprintf(L"WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    //----------------------
    // Create a SOCKET for connecting to server
    ListenSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); //SOCK_STREAM, IPPROTO_TCP
    if (ListenSocket == INVALID_SOCKET) {
        wprintf(L"socket failed with error: %ld\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    //----------------------
    // The sockaddr_in structure specifies the address family,
    // IP address, and port of the server to be connected to.
    serverService.sin_family = AF_INET;
    serverService.sin_addr.s_addr = inet_addr("192.168.88.250");//inet_addr("127.0.0.1");
    serverService.sin_port = htons(DEFAULT_PORT);

    //----------------------
    // Connect to server.
    iResult = bind(ListenSocket, (SOCKADDR*)&serverService, sizeof(serverService));
    if (iResult == SOCKET_ERROR) {
        wprintf(L"connect failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    Timerwp_t TryListeningPeriod;
    InitTimerWP(&TryListeningPeriod, (tickptr_fn*)GetTickCount);
    LaunchTimerWP((U32_ms)1000, &TryListeningPeriod);

    iResult = SOCKET_ERROR;
    do {
        if (IsTimerWPRinging(&TryListeningPeriod)) {
            iResult = listen(ListenSocket, SOMAXCONN);
            if (iResult == SOCKET_ERROR) {
                RestartTimerWP(&TryListeningPeriod);
                printf("listen failed with error: %d\n", WSAGetLastError());
                //closesocket(ListenSocket);
                //WSACleanup();
                //return 1;
            }
        }
    } while (iResult == SOCKET_ERROR);
}