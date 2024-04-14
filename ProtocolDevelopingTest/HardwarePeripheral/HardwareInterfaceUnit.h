#ifndef HARDWARE_INTERFACE_UNIT_H
#define HARDWARE_INTERFACE_UNIT_H
#include <stdint.h>
#include "../../Lib/fileHandle.h"
#include "../../Lib/SimpleTimerWP.h"
#include "../IO_immitationBetweenMasterSlave/MasterImmitationCfg.h"

#ifdef IOFILE_PATH
char iofilePath[200];
#endif // IOFILE_PATH

enum {
	PORT_READY = 1,
	PORT_BUSY = 1 << 1,
	PORT_SENDING = 1 << 2,
	PORT_SENDED = 1 << 3,
	PORT_RECEIVING = 1 << 4, //mb not needed
	PORT_RECEIVED = 1 << 5,
	//..
}InterfacePortState_e;

typedef struct {
	uint8_t BufferRecved[255];
	uint8_t BufferToSend[255];
	uint16_t LenDataToSend;
	uint16_t LenDataToRecv; //mb maxPossibleDataRecv
	Timerwp_t ReceivingTimer;     //for bus timeout
	Timerwp_t SendingTimer;       //this is for describing timeout 
	uint16_t communicationPeriod; //Timerwp_t
	enum InterfacePortState_e Status; //(int)
}InterfacePortHandle_t;

InterfacePortHandle_t InterfacePort; //InterfacePort[ALL_CHANNELS]

int Write(InterfacePortHandle_t *PortHandle, const uint8_t *inBuff, const int size);
int Recv(InterfacePortHandle_t *PortHandle, uint8_t *outBuff, const int maxPossibleSize);
void TransmitInterrupt(void* arg); //Call_TXInterrupt()
void ReceiveInterrupt(void* arg);
//int SentInterrupt(void); //End of transmit callback


#endif // !HARDWARE_INTERFACE_UNIT_H