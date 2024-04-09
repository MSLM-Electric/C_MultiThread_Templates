#ifndef HARDWARE_INTERFACE_UNIT_H
#define HARDWARE_INTERFACE_UNIT_H
#include <stdint.h>
#include "../../Lib/fileHandle.h"
#include "../../Lib/SimpleTimerWP.h"

#ifdef IOFILE_PATH
char iofilePath[200];
#endif // IOFILE_PATH

typedef struct {
	uint8_t BufferRecved[255];
	uint8_t BufferToSend[255];
	Timerwp_t ReceivingTimer;
	Timerwp_t SendingTimer;
	int Status;
}InterfacePortHandle_t;

InterfacePortHandle_t InterfacePort;

int Write(void *PortHandle, const uint8_t inBuff, const int size);
int Recv(void *PortHandle, uint8_t outBuff, const int maxPossibleSize);
int TransmitInterrupt(void* arg);
int ReceiveInterrupt(void* arg);
//int SentInterrupt(void); //End of transmit callback


#endif // !HARDWARE_INTERFACE_UNIT_H