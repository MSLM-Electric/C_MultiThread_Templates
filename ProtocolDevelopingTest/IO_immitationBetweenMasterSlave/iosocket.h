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

SOCKET ConnectSocket;
SOCKET ListenSocket;
/*for portability*/
/*typedef HANDLE mutex_p;*/
#define maxDELAY INFINITE
HANDLE SocketMutex;

DWORD WINAPI ioclientsock_task(LPVOID lpParam);
ThreadsStruct_t ioclientsock_struct;
int CreateClientAndConnect(void);
DWORD WINAPI ioserversock_task(LPVOID lpParam);
ThreadsStruct_t ioserversock_struct;
int CreateServerAndListen(void);

#endif //IOSOCKET_H