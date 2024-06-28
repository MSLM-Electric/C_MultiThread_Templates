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
#ifdef WSA_IN_MAIN_INIT_SECTION
    ListenSocket = WSASocketW(AF_INET, SOCK_DGRAM, IPPROTO_UDP, 0, 0, WSA_FLAG_OVERLAPPED);
#endif
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
    LaunchTimerWP((U32_ms)3000, &ioserverRecvPeriod);

    SOCKADDR_IN remoteNodeAddr;
    int remoteNodeAddrSize = sizeof(remoteNodeAddr);
    remoteNodeAddrSize = sizeof(serverService);
    char buffer[300];
    int res = 0;
    int siz = sizeof(buffer);
    //sprintf(buffer, "server respond");

#ifdef WSA_IN_MAIN_INIT_SECTION
{
    WSABUF wsabuf;
    WSAEVENT Event;
    WSAOVERLAPPED overl;
    Event = WSACreateEvent();
    wsabuf.buf = buffer; //буфер для приема
    wsabuf.len = sizeof(buffer);
    memset(buffer, 0, sizeof(buffer));
}
#endif
    fd_set set;
    struct timeval timeout;
    FD_ZERO(&set); /* clear the set */
    FD_SET(ListenSocket, &set); /* add our file descriptor to the set */
    timeout.tv_sec = 3;
    timeout.tv_usec = 0;

    for (;;)
    {
        //RestartTimerWP(&ioserverResponsePeriod);
        //while (NOT IsTimerWPRinging(&ioserverResponsePeriod));
        RestartTimerWP(&ioserverRecvPeriod);
        
        do {
            //res = recvfrom(ListenSocket, buffer, sizeof(buffer), 0, &serverService, &remoteNodeAddrSize); //?strlen(buffer) is dangerous when receiving
            int res = recvWithTimeout(ListenSocket, &set, buffer, sizeof(buffer), &timeout, &serverService, &remoteNodeAddrSize);
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

int recvWithTimeout(SOCKET ListenSocket, fd_set *readfds, char* buffer, int buffLen, const TIMEVAL* timeout, SOCKADDR_IN *serverService, int *remoteNodeAddrSize)
{
    int res = 0;
    int recvSize = 0;
    res = select(ListenSocket + 1, readfds, NULL, NULL, timeout);
    switch (res)
    {
    case SOCKET_ERROR:
        printf("sock error!\n");
        FD_SET(ListenSocket, readfds); //+!!
        //closesocket(ListenSocket);
        //res = bind(ListenSocket, serverService, remoteNodeAddrSize);
        break;
    case 0:
        printf("timeout occured\n");
        break;
    default:
        recvSize = recvfrom(ListenSocket, buffer, buffLen, 0, serverService, remoteNodeAddrSize);
        if (recvSize == SOCKET_ERROR)
            printf("read failed\n");
        else if (recvSize == 0)
            printf("peer disconnected\n");
        else {
            printf("read successful!\n");
        }
        break;
    }
    return res;
}

#ifdef WSA_IN_MAIN_INIT_SECTION
int recvWithTimeout(SOCKET ListenSocket, WSABUF *wsabuf, WSAEVENT Event, WSAOVERLAPPED *overl, DWORD timeout)
{
    int res = 0;
    memset(overl, 0x0, sizeof(WSAOVERLAPPED));
    overl->hEvent = *(HANDLE*)Event;
    //overl->hEvent = Event;
    DWORD recb = wsabuf->len, flag = 0, sr = 0;
    res = WSARecv(ListenSocket, wsabuf, 1, &recb, &flag, &overl, 0);
    WSAWaitForMultipleEvents(1, Event, FALSE, timeout, FALSE); //wait 20 secs.
    WSAResetEvent(Event);
    WSAGetOverlappedResult(ListenSocket, &overl, &sr, FALSE, &flag);
    return res;
}
#endif