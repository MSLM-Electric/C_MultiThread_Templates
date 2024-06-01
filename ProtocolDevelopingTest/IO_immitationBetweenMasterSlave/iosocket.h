#ifndef IOSOCKET_H
#define IOSOCKET_H

#include <stdint.h>
#include "../../MultiThreadSupport.h"
#include "../../Lib/SimpleTimerWP.h"
#include "../../Lib/type_def.h"

DWORD WINAPI ioclientsock_task(LPVOID lpParam);
ThreadsStruct_t ioclientsock_struct;
DWORD WINAPI ioserversock_task(LPVOID lpParam);
ThreadsStruct_t ioserversock_struct;

#endif //IOSOCKET_H