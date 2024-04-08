#ifndef HARDWARE_INTERFACE_UNIT_H
#define HARDWARE_INTERFACE_UNIT_H
#include <stdint.h>
#include "../../Lib/fileHandle.h"

#ifdef IOFILE_PATH
char iofilePath[200] = IOFILE_PATH;
#endif // IOFILE_PATH


int Write(void *PortHandle, const uint8_t inBuff, const int size);
int Recv(void *PortHandle, uint8_t outBuff, const int maxPossibleSize);
int TransmitInterrupt(void* arg);
int ReceiveInterrupt(void* arg);
//int SentInterrupt(void); //End of transmit callback


#endif // !HARDWARE_INTERFACE_UNIT_H