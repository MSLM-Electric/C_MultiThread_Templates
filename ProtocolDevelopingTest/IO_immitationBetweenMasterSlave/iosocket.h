#ifndef IOSOCKET_H
#define IOSOCKET_H

//#define _WINSOCKAPI_
#include <stdint.h>
#include <WinSock2.h>
#include "../../MultiThreadSupport.h"
#include "../../Lib/SimpleTimerWP.h"
#include "../../Lib/type_def.h"


//// Link with ws2_32.lib
//#pragma comment(lib, "Ws2_32.lib")

/*for portability*/
/*typedef HANDLE mutex_p;*/
#define maxDELAY INFINITE
HANDLE SocketMutex;

typedef struct {
    SOCKET Socket;
    struct sockaddr_in interfaceService;
    fd_set set;
    int remoteNodeAddrSize;
}SocketInerfaceParams_t;

SocketInerfaceParams_t ConnectSocketIfs;
SocketInerfaceParams_t ListenSocketIfs;

SOCKET ConnectSocket; //!not needed
SOCKET ListenSocket;  //!not needed

DWORD WINAPI ioclientsock_task(LPVOID lpParam);
ThreadsStruct_t ioclientsock_struct;
int CreateClientSocket(void);
DWORD WINAPI ioserversock_task(LPVOID lpParam);
ThreadsStruct_t ioserversock_struct;
int CreateServerSocket(void);
int recvWithTimeoutToServer(char* buffer, const int buffLen, const U32_ms timeout);
int recvWithTimeoutToClient(char* buffer, const int buffLen, const U32_ms timeout);
int recvWithTimeout(SOCKET Socket, fd_set* readfds, char* buffer, int buffLen, const TIMEVAL* timeout, SOCKADDR_IN* serverService, int* remoteNodeAddrSize);
U32_ms ConvertTo_ms(struct timeval timeValue);
struct timeval ConvertTo_timeval(const U32_ms u32val);
#ifdef WSA_IN_MAIN_INIT_SECTION
int recvWithTimeout(SOCKET ListenSocket, WSABUF *wsabuf, WSAEVENT Event, WSAOVERLAPPED* overl, DWORD timeout);
#endif //!WSA_IN_MAIN_INIT_SECTION
#endif //IOSOCKET_H