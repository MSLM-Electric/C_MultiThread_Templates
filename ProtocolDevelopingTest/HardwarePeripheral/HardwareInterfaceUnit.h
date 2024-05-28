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

#define no_required_now 0
#pragma region HARDWARE_PORT
#define IN_CASE_OF_FIFO_TYPE
typedef struct {
	u8 TXInterruptEnable;
	u8 RXInterruptEnable;
	u32 someSettings; //Common Inits
	u8 BUFFER;
	u8 StartTX;
	u8 StartRX;
	u8 clearOrResetSomeFlags;
#ifdef IN_CASE_OF_FIFO_TYPE
	u8 FIFO_BUFFER[255];
	u8 clearFIFO;
#endif //!IN_CASE_OF_FIFO_TYPE
}HardwarePort_t;
HardwarePort_t HWPort;
#pragma endregion

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


typedef int* (DelayedRecv_fn)(void* ifsPort, u16 maxPossibleLen);
typedef struct {
	DelayedRecv_fn* DelayedRecv;
	void* ifsArg;
	u16 maxLen;
}DelayedRecv_t;

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
	u16 errCnt;
	DelayedRecv_t DelayedRecv;
}InterfacePortHandle_t;

extern InterfacePortHandle_t InterfacePort; //InterfacePort[ALL_CHANNELS] //InterfacePort[PORT0];
int InitPort(InterfacePortHandle_t* PortHandle);
int Write(InterfacePortHandle_t *PortHandle, const uint8_t *inDatas, const int size);
int Recv(InterfacePortHandle_t *PortHandle, uint8_t *outBuff, const int maxPossibleSize);
int SendingHandle(InterfacePortHandle_t* Port);
int ReceivingHandle(InterfacePortHandle_t* Port);
void TransmitInterrupt(void* arg); //Call_TXInterrupt()
void Called_RXInterrupt(void* arg); //void ReceiveInterrupt(void* arg);
//int SentInterrupt(void); //End of transmit callback
int immitationReceivingOfPortsBus(InterfacePortHandle_t* outPortHandle);
#ifdef GLOB_MUTEX_FILE
FRESULT TakeGLOBMutex(/*char* tempBuffer, const size_t maxPossibleLen,*/ FIL* f, uint32_t timeOut);
FRESULT RealeaseGLOBMutex(FIL* f);
#endif // GLOB_MUTEX_FILE
#define ACCUMUL_CAPACITY 200
typedef struct {
	Timerwp_t TraceTime; //TraceTimeToAccum
	Timerwp_t FrequencyTrace;
	u16 accumulatedStats[ACCUMUL_CAPACITY];
	u16 accumArrayPos;
}tracePortCfg_t;
tracePortCfg_t PortTracer;
void tracePortInit(tracePortCfg_t* traceP);
void tracePort(InterfacePortHandle_t* Port, tracePortCfg_t* traceP);
void ShowTracedAccumulations(tracePortCfg_t* traceP);
u16 BitPos(u16 Bit);


#endif // !HARDWARE_INTERFACE_UNIT_H