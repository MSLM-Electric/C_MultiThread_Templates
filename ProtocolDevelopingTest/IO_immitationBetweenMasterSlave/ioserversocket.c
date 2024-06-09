#ifndef UNICODE
#define UNICODE
#endif

#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>
#include <Ws2tcpip.h>
#include <stdio.h>
#include "iosocket.h"

// Link with ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT 502//27015//

Timerwp_t ioserverResponsePeriod;

DWORD WINAPI ioserversock_task(LPVOID lpParam)
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
    ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); //IPPROTO_UDP
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

    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    InitTimerWP(&ioserverResponsePeriod, (tickptr_fn*)GetTickCount);
    LaunchTimerWP((U32_ms)1000*20, &ioserverResponsePeriod);

    for (;;)
    {
        RestartTimerWP(&ioserverResponsePeriod);
        while (NOT IsTimerWPRinging(&ioserverResponsePeriod));
        //----------------------
        // Send an initial buffer
        iResult = send(ListenSocket, sendbuf, (int)strlen(sendbuf), 0);
        if (iResult == SOCKET_ERROR) {
            wprintf(L"send failed with error: %d\n", WSAGetLastError());
            closesocket(ListenSocket);
            WSACleanup();
            return 1;  //?! Exits in here!
        }

        printf("Bytes Sent: %d\n", iResult);

        // shutdown the connection since no more data will be sent
        iResult = shutdown(ListenSocket, SD_SEND);
        if (iResult == SOCKET_ERROR) {
            wprintf(L"shutdown failed with error: %d\n", WSAGetLastError());
            closesocket(ListenSocket);
            WSACleanup();
            return 1;
        }

        // Receive until the peer closes the connection
        do {

            iResult = recv(ListenSocket, recvbuf, recvbuflen, 0);
            if (iResult > 0)
                wprintf(L"Bytes received: %d\n", iResult);
            else if (iResult == 0)
                wprintf(L"Connection closed\n");
            else
                wprintf(L"recv failed with error: %d\n", WSAGetLastError());

        } while (iResult > 0);


        // close the socket
        iResult = closesocket(ListenSocket);
        if (iResult == SOCKET_ERROR) {
            wprintf(L"close failed with error: %d\n", WSAGetLastError());
            WSACleanup();
            return 1;
        }

        WSACleanup();
    }
    return 0;
}
