#include "fileHandle.h"

//DWORD WINAPI GetFileSize(__in HANDLE hFile, __out_opt LPDWORD lpFileSizeHigh);


FSIZE_t getSizeOfFile(FIL *fp)
{    
    //return  f_size(fp);
    FSIZE_t fileSize = 0;
    LPDWORD lpFileSizeHigh = &fileSize;
    return GetFileSize(fp, lpFileSizeHigh);
}


FRESULT ReadFileTillLine(char* outBuffer, const size_t maxPossibleLen, FIL* Stream)
{
    FRESULT fres = FR_OK;
    char* estr = f_gets(outBuffer, maxPossibleLen, Stream);
    if (estr == NULL)
    {
        if (!f_eof(Stream) == NULL)
        {
#ifndef DEBUG_ON_VS
            fres = FR_INVALID_PARAMETER;//~FR_OK;
#endif // DEBUG_ON_VS             
        }
    }
    return fres; //FR_OK
}

//before calling this function file should be open
FRESULT ReadTheLastLineOfFile(char* outBuffer, const size_t maxPossibleLen, FIL* f)
{
    FRESULT fres;
    if (f != NULL)
    {
        FSIZE_t cursorPosInFile = 0;
        if((f_size(f) - 1024L - 128L) > f_size(f)) //overcrossing from zero to highest value protection
            cursorPosInFile = 0;
        else
            cursorPosInFile = f_size(f) - 1024L - 128L;
        f_lseek(f, cursorPosInFile/*, SEEK_END*/); //in QT stm32 fatfs: res = f_lseek(f, f_size(f) - 1024L-128L);
        //long int res = ftell(f);

        size_t lastStringsLen = 0;
        int16_t timeOut = (int16_t)(maxPossibleLen) + 20;
        while ((f_eof(f) == NULL) && (timeOut > 0)) {
            fres = ReadFileTillLine(outBuffer, maxPossibleLen, f);
            lastStringsLen = strlen(outBuffer);  //? in here once freezed. put the timeout to get out from here, or look at f.obj.stat is it == FR_OK?
            timeOut--; //or look also at f.err /f.flag  /f.obj.lockid
        };
        if (fres != FR_OK)
        {
            return fres;
        }
        if (lastStringsLen == maxPossibleLen)
        {
            //printf("maybe length of line too long\n"); //test it osm
        }
        f_lseek(f, f_size(f) - lastStringsLen/*, SEEK_END*/); //set the pointer of file to the (end - laststr)
        fres = ReadFileTillLine(outBuffer, maxPossibleLen, f);
    }
    f_close(f);
    return fres;
}


#ifdef GLOB_MUTEX_FILE
#define NOT !
#include "SimpleTimerWP.h"
FRESULT TakeGLOBMutex(/*char* tempBuffer, const size_t maxPossibleLen,*/ FIL *f, uint32_t timeOut)
{
    Timerwp_t _TimeOut, readPeriod;
    char buffer[100];
    InitTimerWP(&_TimeOut, (tickptr_fn*)GetTickCount);
    InitTimerWP(&readPeriod, (tickptr_fn*)GetTickCount);
    LaunchTimerWP((U32_ms)timeOut, &_TimeOut);
    LaunchTimerWP((U32_ms)10, &readPeriod);
    FRESULT fres = FR_OK; //FR_NOT_READY;
    do {
        f = fopen(GLOB_MUTEX_FILE, "r+");
        if (f == NULL) {
            fres = FR_NOT_READY;
            return fres;
        }
        memset(buffer, 0, sizeof(buffer));
        fres = ReadFileTillLine(buffer, sizeof(buffer), f);
        if ((fres == FR_OK) && (strncmp(buffer, FILE_MUTEX_REALEASED, strlen(FILE_MUTEX_REALEASED)) == 0)) {
            //f_lseek(f, 0);
            fseek(f, 0, SEEK_SET);
            sprintf(buffer, "%s    ", (char*)FILE_MUTEX_TAKEN);
            size_t siz = fwrite(buffer, strlen(buffer), 1, f);
            StopTimerWP(&_TimeOut);
            StopTimerWP(&readPeriod);
            f_close(f);
            return fres;
        }
        f_close(f);
        while (NOT IsTimerWPRinging(&readPeriod));
        RestartTimerWP(&readPeriod);
    } while (NOT IsTimerWPRinging(&_TimeOut));
    f_close(f);
    StopTimerWP(&_TimeOut);
    StopTimerWP(&readPeriod);
    return FR_NOT_READY;
}

FRESULT RealeaseGLOBMutex(FIL* f)
{
    FRESULT fres = FR_OK;
    f = fopen(GLOB_MUTEX_FILE, "r+");
    if (f == NULL) {
        fres = FR_TOO_MANY_OPEN_FILES;
        return fres;
    }
    char buffer[100];
    memset(buffer, 0, sizeof(buffer));
    fres = ReadFileTillLine(buffer, sizeof(buffer), f);
    if ((fres == FR_OK) && (strncmp(buffer, FILE_MUTEX_TAKEN, strlen(FILE_MUTEX_TAKEN)) == 0)) {
        //f_lseek(f, 0);
        fseek(f, 0, SEEK_SET);
        sprintf(buffer, "%s\n", (char*)FILE_MUTEX_REALEASED);
        size_t siz = fwrite(buffer, strlen(buffer), 1, f);
        f_close(f);
        return fres;
    }
    else if ((fres == FR_OK) && (strncmp(buffer, FILE_MUTEX_REALEASED, strlen(FILE_MUTEX_REALEASED)) == 0)) {
        f_close(f);
        return fres;
    }
    f_close(f);
    return FR_NOT_READY;
}
#endif // GLOB_MUTEX_FILE
