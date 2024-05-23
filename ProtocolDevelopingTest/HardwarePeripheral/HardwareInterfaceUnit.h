#ifndef HARDWARE_INTERFACE_UNIT_H
#define HARDWARE_INTERFACE_UNIT_H
#include <stdint.h>
#include "../../Lib/fileHandle.h"
#include "../../Lib/SimpleTimerWP.h"
#ifdef MASTER_PORT_PROJECT
#include "../IO_immitationBetweenMasterSlave/MasterImmitationCfg.h"
#else //if SLAVE_PORT_PROJECT
#include "../IO_immitationBetweenMasterSlave/SlaveImmitationCfg.h"
#endif // MASTER_PORT_PROJECT


#ifdef IOFILE_PATH
char iofilePath[200];
#endif // IOFILE_PATH
#ifdef GLOB_MUTEX_FILE
char globMutexFile[200];
#endif //GLOB_MUTEX_FILE

enum {
	//PORT_OFF = 0, //PORT_CLEAR,
	PORT_READY = 1, //mb PORT_OK
	PORT_BUSY = 1 << 1,      //mb not needed. instead it mb use only receiving flag
	PORT_SENDING = 1 << 2,
	PORT_SENDED = 1 << 3,
	PORT_SENDING_LAST_BYTE = 1 << 4,
	PORT_RECEIVING = 1 << 5, //mb not needed
	PORT_RECEIVED = 1 << 6,
	PORT_ASYNC = 1 << 7,     //if zero, then it is a PORT_SYNC.  PORT_ASYNC = RS-485/CAN like interfaces.
	PORT_MASTER = 1 << 8,    //if zero, it is SLAVE (good approach//?)
	PORT_BUFFER_FIFO = 1 << 9, //if zero, it is simple 8bit buffer //PORT_BUFF_FIFO_ENABLED
	//..
	// PORT_ERROR //?
}InterfacePortState_e;

typedef struct {
	uint8_t BufferRecved[255];
	uint8_t BufferToSend[255];
	uint16_t LenDataToSend;
	uint16_t LenDataToRecv; //mb maxPossibleDataRecv
	Timerwp_t ReceivingTimer;     //for port timeout
	Timerwp_t SendingTimer;       //this is for describing timeout 
	uint16_t communicationPeriod; //!not demanded for slave //Timerwp_t
	//uin32_t TotalCommunicationPeriod; //!not demanded for slave
	enum InterfacePortState_e Status; //(int)
	u16 outCursor; //outPtr; mb outCursorPos; sendbufPos;
	u16 inCursor;  //inPtr; 
}InterfacePortHandle_t;

extern InterfacePortHandle_t InterfacePort; //InterfacePort[ALL_CHANNELS] //InterfacePort[PORT0];

int Write(InterfacePortHandle_t *PortHandle, const uint8_t *inBuff, const int size);
int Recv(InterfacePortHandle_t *PortHandle, uint8_t *outBuff, const int maxPossibleSize);
void TransmitInterrupt(void* arg); //Call_TXInterrupt()
void Called_RXInterrupt(void* arg); //void ReceiveInterrupt(void* arg);
//int SentInterrupt(void); //End of transmit callback
int immitationReceivingOfPortsBus(InterfacePortHandle_t* outPortHandle);
#ifdef GLOB_MUTEX_FILE
FRESULT TakeGLOBMutex(/*char* tempBuffer, const size_t maxPossibleLen,*/ FIL* f, uint32_t timeOut);
FRESULT RealeaseGLOBMutex(FIL* f);
#endif // GLOB_MUTEX_FILE


#endif // !HARDWARE_INTERFACE_UNIT_H