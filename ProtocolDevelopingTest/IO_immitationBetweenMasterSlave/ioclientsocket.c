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
#define DEFAULT_PORT 502//27015//

Timerwp_t ioclientrequestPeriod;
//Timerwp_t Timers[4];

DWORD WINAPI ioclientsock_task(LPVOID lpParam) 
{
    //----------------------
    // Declare and initialize variables.
    int iResult;
    WSADATA wsaData;

    ConnectSocket = INVALID_SOCKET;
    struct sockaddr_in clientService;

    int recvbuflen = DEFAULT_BUFLEN;
    char* sendbuf = "Client: sending data test";
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
    ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); //SOCK_DGRAM, IPPROTO_UDP - in case of udp not required to connect
    if (ConnectSocket == INVALID_SOCKET) {
        wprintf(L"socket failed with error: %ld\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    //----------------------
    // The sockaddr_in structure specifies the address family,
    // IP address, and port of the server to be connected to.
    clientService.sin_family = AF_INET;
    clientService.sin_addr.s_addr = inet_addr("192.168.88.250");//inet_addr("127.0.0.1");
    clientService.sin_port = htons(DEFAULT_PORT);

    //----------------------
    // Connect to server.
    iResult = connect(ConnectSocket, (SOCKADDR*)&clientService, sizeof(clientService));
    if (iResult == SOCKET_ERROR) {
        wprintf(L"connect failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }
    InitTimerWP(&ioclientrequestPeriod, (tickptr_fn*)GetTickCount);
    LaunchTimerWP((U32_ms)1000, &ioclientrequestPeriod);

    for (;;)
    {
        RestartTimerWP(&ioclientrequestPeriod);
        while (NOT IsTimerWPRinging(&ioclientrequestPeriod));
        //----------------------
        // Send an initial buffer
        iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
        if (iResult == SOCKET_ERROR) {
            wprintf(L"send failed with error: %d\n", WSAGetLastError());
            closesocket(ConnectSocket);
            WSACleanup();
            return 1;  //?! Exits in here!
        }

        printf("Bytes Sent: %d\n", iResult);

        // shutdown the connection since no more data will be sent
        iResult = shutdown(ConnectSocket, SD_SEND);
        if (iResult == SOCKET_ERROR) {
            wprintf(L"shutdown failed with error: %d\n", WSAGetLastError());
            closesocket(ConnectSocket);
            WSACleanup();
            return 1;
        }

        // Receive until the peer closes the connection
        do {

            iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
            if (iResult > 0)
                wprintf(L"Bytes received: %d\n", iResult);
            else if (iResult == 0)
                wprintf(L"Connection closed\n");
            else
                wprintf(L"recv failed with error: %d\n", WSAGetLastError());

        } while (iResult > 0);


        // close the socket
        iResult = closesocket(ConnectSocket);
        if (iResult == SOCKET_ERROR) {
            wprintf(L"close failed with error: %d\n", WSAGetLastError());
            WSACleanup();
            return 1;
        }

        WSACleanup();
    }
    return 0;
}


int CreateClientSocket(void)
{
    //----------------------
// Declare and initialize variables.
    int iResult;
    WSADATA wsaData;

    SOCKET ConnectSocket = INVALID_SOCKET;
    struct sockaddr_in clientService;

    int recvbuflen = DEFAULT_BUFLEN;
    char* sendbuf = "Client: sending data test";
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
    ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); //IPPROTO_UDP
    if (ConnectSocket == INVALID_SOCKET) {
        wprintf(L"socket failed with error: %ld\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    //----------------------
    // The sockaddr_in structure specifies the address family,
    // IP address, and port of the server to be connected to.
    clientService.sin_family = AF_INET;
    clientService.sin_addr.s_addr = inet_addr("192.168.88.250");//inet_addr("127.0.0.1");
    clientService.sin_port = htons(DEFAULT_PORT);


    Timerwp_t TryConnectPeriod;
    InitTimerWP(&TryConnectPeriod, (tickptr_fn*)GetTickCount);
    LaunchTimerWP((U32_ms)1000, &TryConnectPeriod);
    //----------------------
    // Connect to server.
    iResult = SOCKET_ERROR;
    do {
        if (IsTimerWPRinging(&TryConnectPeriod)) {
            iResult = connect(ConnectSocket, (SOCKADDR*)&clientService, sizeof(clientService));
            if (iResult == SOCKET_ERROR) {
                RestartTimerWP(&TryConnectPeriod);
                wprintf(L"connect failed with error: %d\n", WSAGetLastError());
                //closesocket(ConnectSocket);
                //WSACleanup();
                //return 1;
            }
        }
    } while (iResult == SOCKET_ERROR);
}

int ClientSend(const u8* buffer, const u16 bufflen, const u16 timeout)
{

}

int ClientRecv(u8* buffRead, u16* sizeRead, const u16 timeout)
{

}