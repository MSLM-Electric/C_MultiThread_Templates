#ifndef _FILEHANDLE_H_
#define _FILEHANDLE_H_

#include <stdio.h>

#define DEBUG_ON_VS
#if defined(DEBUG_ON_VS) || defined(COMPATIBLE_VS)
#include "type_def.h"
#include <Windows.h>

#define f_lseek(x, y) fseek(x, y, SEEK_END)
#define f_gets fgets
#define f_eof  feof
#define f_close fclose

typedef FILE FIL;
typedef unsigned long	DWORD;
typedef DWORD FSIZE_t;

/* File function return code (FRESULT) */

typedef enum {
	FR_OK = 0,				/* (0) Succeeded */
	FR_DISK_ERR,			/* (1) A hard error occurred in the low level disk I/O layer */
	FR_INT_ERR,				/* (2) Assertion failed */
	FR_NOT_READY,			/* (3) The physical drive cannot work */
	FR_NO_FILE,				/* (4) Could not find the file */
	FR_NO_PATH,				/* (5) Could not find the path */
	FR_INVALID_NAME,		/* (6) The path name format is invalid */
	FR_DENIED,				/* (7) Access denied due to prohibited access or directory full */
	FR_EXIST,				/* (8) Access denied due to prohibited access */
	FR_INVALID_OBJECT,		/* (9) The file/directory object is invalid */
	FR_WRITE_PROTECTED,		/* (10) The physical drive is write protected */
	FR_INVALID_DRIVE,		/* (11) The logical drive number is invalid */
	FR_NOT_ENABLED,			/* (12) The volume has no work area */
	FR_NO_FILESYSTEM,		/* (13) There is no valid FAT volume */
	FR_MKFS_ABORTED,		/* (14) The f_mkfs() aborted due to any problem */
	FR_TIMEOUT,				/* (15) Could not get a grant to access the volume within defined period */
	FR_LOCKED,				/* (16) The operation is rejected according to the file sharing policy */
	FR_NOT_ENOUGH_CORE,		/* (17) LFN working buffer could not be allocated */
	FR_TOO_MANY_OPEN_FILES,	/* (18) Number of open files > _FS_LOCK */
	FR_INVALID_PARAMETER	/* (19) Given parameter is invalid */
} FRESULT;
#endif // DEBUG_ON_VS || COMPATIBLE_VS


FSIZE_t getSizeOfFile(FIL *fp);
#define f_size  getSizeOfFile
FRESULT ReadFileTillLine(char* outBuffer, const size_t maxPossibleLen, FIL* Stream);
FRESULT ReadTheLastLineOfFile(char* outBuffer, const size_t maxPossibleLen, FIL* f);

#endif //_FILEHANDLE_H_



//SOME INFOS
/*
Mode flags that specifies the type of access and open method for the file. It is specified by a combination of following flags.
Flags	Meaning
FA_READ					Specifies read access to the file. Data can be read from the file.
FA_WRITE				Specifies write access to the file. Data can be written to the file. Combine with FA_READ for read-write access.
FA_OPEN_EXISTING		Opens a file. The function fails if the file is not existing. (Default)
FA_CREATE_NEW			Creates a new file. The function fails with FR_EXIST if the file is existing.
FA_CREATE_ALWAYS		Creates a new file. If the file is existing, it will be truncated and overwritten.
FA_OPEN_ALWAYS			Opens the file if it is existing. If not, a new file will be created.
FA_OPEN_APPEND			Same as FA_OPEN_ALWAYS except the read/write pointer is set end of the file.

Mode flags in POSIX fopen() function corresponds to FatFs mode flags as follows:
POSIX	FatFs
"r"			FA_READ
"r+"		FA_READ | FA_WRITE
"w"			FA_CREATE_ALWAYS | FA_WRITE
"w+"		FA_CREATE_ALWAYS | FA_WRITE | FA_READ
"a"			FA_OPEN_APPEND | FA_WRITE
"a+"		FA_OPEN_APPEND | FA_WRITE | FA_READ
"wx"		FA_CREATE_NEW | FA_WRITE
"w+x"		FA_CREATE_NEW | FA_WRITE | FA_READ
*/