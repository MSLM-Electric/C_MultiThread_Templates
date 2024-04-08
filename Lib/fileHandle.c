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
    char* estr = f_gets(outBuffer, maxPossibleLen, Stream);
    if (estr == NULL)
    {
        if (!f_eof(Stream) == NULL)
        {
            return FR_INVALID_PARAMETER;//~FR_OK;
        }
    }
    return FR_OK; //FR_OK
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