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

Timerwp_t ioclientrequestPeriod;
Timerwp_t ioclientRecvPeriod;
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
    ConnectSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); //SOCK_DGRAM, IPPROTO_UDP - in case of udp not required to connect
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

    InitTimerWP(&ioclientrequestPeriod, (tickptr_fn*)GetTickCount);
    LaunchTimerWP((U32_ms)2000, &ioclientrequestPeriod);
    InitTimerWP(&ioclientRecvPeriod, (tickptr_fn*)GetTickCount);
    LaunchTimerWP((U32_ms)1000, &ioclientRecvPeriod);
    SOCKADDR_IN remoteNodeAddr;
    int remoteNodeAddrSize = sizeof(remoteNodeAddr);
    remoteNodeAddrSize = sizeof(clientService);
    char buffer[300];
    int res = 0;
    sprintf(buffer, "client send requests!\n\0");

    for (;;)
    {
        RestartTimerWP(&ioclientrequestPeriod);
        while (NOT IsTimerWPRinging(&ioclientrequestPeriod));
        //----------------------
        // Send an initial buffer
        res = sendto(ConnectSocket, buffer, strlen(buffer), 0, &clientService, &remoteNodeAddrSize);
        //if (res == SOCKET_ERROR) {
        //    wprintf(L"send failed with error: %d\n", WSAGetLastError());
        //    closesocket(ConnectSocket);
        //    WSACleanup();
        //    return 1;  //?! Exits in here!
        //}

        printf("Bytes Sent: %d\n", res);

        RestartTimerWP(&ioclientRecvPeriod);
        /*do {
            res = recvfrom(ConnectSocket, buffer, sizeof(buffer), 0, &clientService, &remoteNodeAddrSize);
            if (IsTimerWPRinging(&ioclientRecvPeriod)) {
                printf("client not fully recved");
                res = -1;
                break;
            }

        } while (res > 0);*/
    }
    // close the socket
    iResult = closesocket(ConnectSocket);
    if (iResult == SOCKET_ERROR) {
        wprintf(L"close failed with error: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    WSACleanup();
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