#include "iosocket.h"

int recvWithTimeout(SOCKET ListenSocket, fd_set* readfds, char* buffer, int buffLen, const TIMEVAL* timeout, SOCKADDR_IN* serverService, int* remoteNodeAddrSize)
{
    int res = 0;
    int recvSize = 0;
    uint8_t printingDebugCmd = 1;
    stopwatchwp_t selectMeasure;
    InitStopWatchWP(&selectMeasure, (tickptr_fn*)GetTickCount);
    StopWatchWP(&selectMeasure);
    res = select(ListenSocket + 1, readfds, NULL, NULL, timeout);
    switch (res)
    {
    case SOCKET_ERROR:
        DEBUG_PRINTF(printingDebugCmd, ("sock error!\n"));
        FD_SET(ListenSocket, readfds); //+!!
        //closesocket(ListenSocket);
        //res = bind(ListenSocket, serverService, remoteNodeAddrSize);
        break;
    case 0:
        DEBUG_PRINTF(printingDebugCmd, ("timeout occured\n"));
        break;
    default:
        recvSize = recvfrom(ListenSocket, buffer, buffLen, 0, serverService, remoteNodeAddrSize);
        if (recvSize == SOCKET_ERROR)
            DEBUG_PRINTF(printingDebugCmd, ("read failed\n"));
        else if (recvSize == 0)
            DEBUG_PRINTF(printingDebugCmd, ("peer disconnected\n"));
        else {
            DEBUG_PRINTF(printingDebugCmd, ("read successful!\n"));
        }
        break;
    }
    StopWatchWP(&selectMeasure);
    DEBUG_PRINTF(printingDebugCmd, ("sockets select func measure:%d\n", selectMeasure.measuredTime));
    return res;
}

U32_ms ConvertTo_ms(struct timeval timeValue)
{
    U32_ms res = timeValue.tv_sec * 1000 + timeValue.tv_usec/1000;
    return res;
}

struct timeval ConvertTo_timeval(const U32_ms u32val)
{
    struct timeval resVal;
    resVal.tv_sec = u32val / 1000;
    resVal.tv_usec = (u32val - resVal.tv_sec * 1000) * 1000;
    return resVal;
}