#ifndef IOSOCKET_H
#define IOSOCKET_H

#include <stdint.h>
#include <winsock2.h>
#include "../../MultiThreadSupport.h"
#include "../../Lib/SimpleTimerWP.h"
#include "../../Lib/type_def.h"

SOCKET ConnectSocket;
SOCKET ListenSocket;

DWORD WINAPI ioclientsock_task(LPVOID lpParam);
ThreadsStruct_t ioclientsock_struct;
int CreateClientAndConnect(void);
DWORD WINAPI ioserversock_task(LPVOID lpParam);
ThreadsStruct_t ioserversock_struct;
int CreateServerAndListen(void);

#endif //IOSOCKET_H