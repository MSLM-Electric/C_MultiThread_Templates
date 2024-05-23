#include "HardwareInterfaceUnit.h"
#include "../IO_immitationBetweenMasterSlave/PortsBusMessages.h"
#include "../../MultiThreadSupport.h"

char mastersMessageId[] = MASTER_MESSAGE_ID;
char slavesMessageId[] = SLAVE_MESSAGE_ID;
HANDLE iofileMutex;
FIL FileHandle;
FIL MutexFileHandle;

#define ONLY //just nothing. Only for clarifying ports state currently

int InitPort(InterfacePortHandle_t* PortHandle)
{
	int res = 0;
	iofileMutex = CreateMutexW(NULL, 1, "IOFILE_Mutex");
	ReleaseMutex(iofileMutex);
	PortHandle->Status = 0;
#ifdef MASTER_PORT_PROJECT
	PortHandle->Status |= PORT_MASTER;
#endif // MASTER_PORT_PROJECT
	InitTimerWP(&PortHandle->ReceivingTimer, (tickptr_fn*)GetTickCount);
	InitTimerWP(&PortHandle->SendingTimer, (tickptr_fn*)GetTickCount);
	PortHandle->ReceivingTimer.setVal = (U32_ms)200; //Default 200ms
	PortHandle->SendingTimer.setVal = (U32_ms)200; //def
	return res;
}

int Write(InterfacePortHandle_t* PortHandle, const uint8_t *inBuff, const int size)
{
	int res = -1;
	if ((PortHandle->Status & (PORT_READY | PORT_SENDING | PORT_RECEIVING /*| PORT_ASYNC*/)) == ONLY (PORT_READY /*| PORT_ASYNC*/)) {
		PortHandle->Status |= PORT_SENDING;
		//memcpy(PortHandle->BufferToSend, inBuff, size);
		//Launch SendingTimer;
		res = immitationOfPortsBus(PortHandle);
		if (res < 0) {
			//PORT_ERROR//?;
		}else{
			;;
		}
	}
	else if((PortHandle->Status & (PORT_READY | PORT_SENDING /*| PORT_ASYNC*/)) == ONLY(PORT_READY | PORT_SENDING)) {
		RestartTimerWP(&PortHandle->SendingTimer);
	}
	else {
		res = -4;
	}
	return res;
}

int Recv(InterfacePortHandle_t* PortHandle, uint8_t *outBuff, const int maxPossibleSize)
{
	int res = 0;
	if ((PortHandle->Status & (PORT_READY | PORT_RECEIVING)) == ONLY PORT_READY) {
		//Enable Hardware RX Interrupt
		PortHandle->Status |= PORT_RECEIVING;
		PortHandle->Status &= ~PORT_RECEIVED;
		//RestartTimerWP(&PortHandle->ReceivingTimer);
		return res;
	}
}

static int immitationOfPortsBus(InterfacePortHandle_t* PortHandle) //! immitationSendingOfPortsBus()
{
	int res = 0;
	FRESULT fres = FR_OK;
	fres = TakeGLOBMutex(&MutexFileHandle, INFINITE);
	if (fres != FR_OK)
		return res = -3;
	char buffer[300];
	//char portsBusMessageId = portsMessageId;
	char* DirectionSendingOfBusMessageId;
	if (PortHandle->Status & PORT_MASTER) {
		DirectionSendingOfBusMessageId = mastersMessageId;
	}else {
		DirectionSendingOfBusMessageId = slavesMessageId;
	}
	//"%s%s %s\n"
	sprintf(buffer, "%s %s\n", DirectionSendingOfBusMessageId, PortHandle->BufferToSend); //sizes?
	TakeMutex(&iofileMutex, INFINITE);
	FIL* f = &FileHandle;
	f = fopen(iofilePath, "a+"); //a
	if (f == NULL) {
		//fclose(f);
		ReleaseMutex(iofileMutex);
		res = -1;
		return res;
	}
	size_t siz = fwrite(buffer, strlen(buffer), 1/*??*/, f);// fwprintf, .._s
	fclose(f);
	ReleaseMutex(iofileMutex);
	RealeaseGLOBMutex(&MutexFileHandle);
	res = (int)siz;
	commonMasterSlaveCfgs_t* currentObjCfg;
#ifdef MASTER_PORT_PROJECT
	currentObjCfg = &ThisMastersConfigs;
#else
	currentObjCfg = &ThisSlavesConfigs;
#endif // MASTER_PORT_PROJECT
	if (res > 0) {
		currentObjCfg/*[PortNo]*/->currentIOfileLine++;
		TransmitInterrupt(PortHandle); //Called_TXInterrupt()
	}
	return res;
}

int immitationReceivingOfPortsBus(InterfacePortHandle_t* outPortHandle)
{
	int res = 0;
	int PortNo = 0;
	FRESULT fres = FR_OK;
	fres = TakeGLOBMutex(&MutexFileHandle, INFINITE);
	if (fres != FR_OK)
		return res = -3;
	char buffer[300];
	//char portsBusMessageId = portsMessageId;

	TakeMutex(&iofileMutex, INFINITE);
	FIL* f = &FileHandle;
	f = fopen(iofilePath, "r"); //!+ fopen_s(&file, fname, "r")  use this, more safely
	if (f == NULL) {
		//fclose(f);
		ReleaseMutex(iofileMutex);
		res = -1;
		return res;
	}
	fres = ReadTheLastLineOfFile(buffer, sizeof(buffer), f);
	ReleaseMutex(iofileMutex);
	RealeaseGLOBMutex(&MutexFileHandle);
	/*if (strncmp(buffer, portsMessageId, strlen(portsMessageId) - 2) == 0) {
		buffer[strlen(portsMessageId) - 2] = PortNo;
	}*/
	char* DirectionSendingOfBusMessageId;
	size_t MessageIdlen = 0;
	if (outPortHandle->Status & PORT_MASTER) {
		DirectionSendingOfBusMessageId = slavesMessageId; //Receiving from SLAVE
		MessageIdlen = strlen(slavesMessageId);
	}else{
		DirectionSendingOfBusMessageId = mastersMessageId;
		MessageIdlen = strlen(mastersMessageId);
	}
	if ((fres == FR_OK) && (strncmp(buffer, DirectionSendingOfBusMessageId, MessageIdlen) == 0)) {
		if (outPortHandle->Status && PORT_MASTER && (DirectionSendingOfBusMessageId == slavesMessageId)) {
#ifdef MASTER_PORT_PROJECT
			if (ThisMastersConfigs.lastReadedLine != ThisMastersConfigs.currentIOfileLine) {
				//Port detected a datas on Bus 
				//we pretend that the Hardware has the big FIFO
				//The copying should be but not in here. This is the immitation of interrupt section
				//on bus/port by detecting datas on there. The copying must be occure on inside interrupt section. 
				memcpy_s(InterfacePort.BufferRecved, sizeof(InterfacePort.LenDataToRecv), &buffer[strlen(slavesMessageId)], sizeof(InterfacePort.LenDataToRecv));
				ThisMastersConfigs.lastReadedLine = ThisMastersConfigs.currentIOfileLine++;
				//Called_RXInterrupt(&InterfacePort);
			}
#endif // MASTER_PORT_PROJECT
		}
		else if ((DirectionSendingOfBusMessageId == mastersMessageId)) {
			;
		}
		Called_RXInterrupt(&InterfacePort);
	}
	else { //?
		res = (int)fres;
		RestartTimerWP(&outPortHandle->ReceivingTimer); //? NOPE! delete it!
	}
	return res;
}

void TransmitInterrupt(void *arg) 
{
	InterfacePortHandle_t* Port = (InterfacePortHandle_t *)arg;
	Port->Status &= ~PORT_SENDING;
	Port->Status |= PORT_SENDED;
	StopTimerWP(&Port->SendingTimer);
}

void Called_RXInterrupt(void* arg) //ReceiveInterrupt()
{
	InterfacePortHandle_t* Port = (InterfacePortHandle_t*)arg;
	Port->Status |= PORT_RECEIVED;
#ifdef MASTER_PORT_PROJECT
	if (InterfacePort.BufferRecved[0] == ThisMastersConfigs.SlavesAddressToTalk){ //IsThisDataForMe();
		;
	}
#endif // MASTER_PORT_PROJECT
	return;
}

#pragma region BLACKNOTE_AND_THOUGHTS
//#define DISABLE_BLACKNOTE
#ifndef DISABLE_BLACKNOTE
#define FEW_ONLY          //?
#define ALL_ONLY FEW_ONLY //?
#define IN_CASE_OF_8BIT_PORTION_DATAS
#define IN_CASE_OF_FIFO_TYPE //disable it after
#define no_required_now 0
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
#ifdef IN_CASE_OF_8BIT_PORTION_DATAS //mb IN_CASE_OF_8BIT_SINGLE_BUFFER
//send
_Write(u8 *InDatas, u16 Len) //Write()
{
	u16 all = 0;
	u16 res = 0;
	if (InterfacePort.Status & (PORT_READY | PORT_SENDING | PORT_BUSY) == ONLY PORT_READY) {
		memcpy(InterfacePort.BufferToSend, InDatas, Len);
		InterfacePort.LenDataToSend = Len;
		InterfacePort.outCursor = 0;
		InterfacePort.outCursor++;
		HWPort.someSettings = 0xFF;
		HWPort.BUFFER = InterfacePort.BufferToSend[0];
		HWPort.TXInterruptEnable = 1;
		HWPort.StartRX = 0;
		HWPort.StartTX = 1;
		InterfacePort.Status |= PORT_BUSY;
		InterfacePort.Status |= PORT_SENDING;
		InterfacePort.Status &= ~PORT_SENDED;
		LaunchTimerWP(InterfacePort.SendingTimer.setVal, &InterfacePort.SendingTimer);
	}
	else if (InterfacePort.Status & (all = PORT_BUSY | PORT_SENDING_LAST_BYTE /* | ???*/) == all ONLY) {
		HWPort.clearOrResetSomeFlags = 0;
		HWPort.TXInterruptEnable = 0;
		HWPort.BUFFER = 0;
		HWPort.someSettings = 0xFF; //mb
		InterfacePort.Status clearBITS(PORT_SENDING_LAST_BYTE | PORT_SENDING | PORT_BUSY); //?whaaaa!?
		InterfacePort.Status &= ~(PORT_SENDING_LAST_BYTE | PORT_SENDING | PORT_BUSY);
		InterfacePort.outCursor = 0;
		StopTimerWP(&InterfacePort.SendingTimer);
	}
	else if (InterfacePort.Status & (all = PORT_BUSY | PORT_SENDED) == all ONLY) { //?is it has good speed execution by MCU processor if we call it from inside of interrupt?
		HWPort.clearOrResetSomeFlags = 0;
		if (InterfacePort.outCursor >= InterfacePort.LenDataToSend - 1)
			InterfacePort.Status |= PORT_SENDING_LAST_BYTE;
		HWPort.BUFFER = &InterfacePort.BufferToSend[InterfacePort.outCursor];
		InterfacePort.outCursor++;
		InterfacePort.Status &= ~PORT_SENDED;
		InterfacePort.Status |= PORT_SENDING;
		HWPort.StartTX = 1;
	}
	//else {
	//	res = -4;
	//}
	if (InterfacePort.Status & PORT_SENDING)
		res = InterfacePort.outCursor;
	//return res;
}

_Receive(InterfacePortHandle_t *ifsPort, /*const u8* outBuff, mb not needed even//?!*/ u16 maxPossibleLen)
{
	u16 both = 0, all = 0, res = 0;
	u8 IsRecvTimerRinging = IsTimerWPRinging(&InterfacePort.ReceivingTimer);
	if (InterfacePort.Status & (PORT_READY | PORT_RECEIVING | PORT_BUSY) == ONLY PORT_READY) {
		InterfacePort.inCursor = 0;
		InterfacePort.LenDataToRecv = maxPossibleLen;
		LaunchTimerWP(InterfacePort.ReceivingTimer.setVal, &InterfacePort.ReceivingTimer);
		InterfacePort.Status |= PORT_BUSY | PORT_RECEIVING;
		InterfacePort.Status &= ~PORT_RECEIVED;
		HWPort.clearOrResetSomeFlags = 0;
		HWPort.someSettings = 0xFF;
		//IsTheseDatasForMe;
		HWPort.RXInterruptEnable = 1;
		HWPort.StartRX = 1;
		//HWPort.StartTX = 0;
	}
	else if((InterfacePort.Status & ((both = PORT_BUSY | PORT_RECEIVED))) && !IsRecvTimerRinging == both ONLY)
	{
		HWPort.clearOrResetSomeFlags = 0;
		HWPort.RXInterruptEnable = 0;
		HWPort.someSettings = 0xff;

		//Recv timeouts
		InterfacePort.BufferRecved[InterfacePort.inCursor++] = HWPort.BUFFER;
		InterfacePort.Status clearBITS(PORT_RECEIVED);
		HWPort.RXInterruptEnable = 1;
		HWPort.StartRX = 1;
	}
	else {
		res = -4;
	}
	if (IsRecvTimerRinging && (InterfacePort.Status & PORT_RECEIVING)) { //?mb
		HWPort.clearOrResetSomeFlags = 0;
		HWPort.RXInterruptEnable = 0;
		HWPort.StartRX = 0;
		HWPort.someSettings = 0xff;
		StopTimerWP(&InterfacePort.ReceivingTimer);
		InterfacePort.Status clearBITS(PORT_RECEIVING | PORT_BUSY);
		InterfacePort.Status |= PORT_RECEIVED; //? mb RECEIVED_TIMEOUT or RECEIVED_ALL?
		InterfacePort.LenDataToRecv = InterfacePort.inCursor;
	}
	return res;
}
#endif //IN_CASE_OF_8BIT_PORTION_DATAS

#ifdef IN_CASE_OF_FIFO_TYPE
__Write(u8* InDatas, u16 Len)
{
	int res = 0;
	//IsSendingTimerRinging = IsTimerWPRinging(&InterfacePort.SendingTimer);
	if (InterfacePort.Status & (PORT_READY | PORT_SENDING | PORT_BUSY) == ONLY PORT_READY) {
		memcpy(InterfacePort.BufferToSend, InDatas, Len);
		InterfacePort.LenDataToSend = Len;
		//InterfacePort.outCursor++;
		memcpy(HWPort.FIFO_BUFFER, InterfacePort.BufferToSend, InterfacePort.LenDataToSend);
		HWPort.someSettings = 0xFF;
		HWPort.TXInterruptEnable = 1;
		HWPort.StartRX = 0;
		HWPort.StartTX = 1;
		InterfacePort.Status |= PORT_BUSY;
		InterfacePort.Status |= PORT_SENDING;
		InterfacePort.Status &= ~PORT_SENDED;
		LaunchTimerWP(InterfacePort.SendingTimer.setVal, &InterfacePort.SendingTimer);
	}else if (InterfacePort.Status & (PORT_BUSY | PORT_SENDING_LAST_BYTE) == ONLY PORT_BUSY | PORT_SENDING_LAST_BYTE) {
		;;
	}else if (InterfacePort.Status & (PORT_BUSY | PORT_SENDED) == ONLY PORT_BUSY | PORT_SENDED) {
		HWPort.clearOrResetSomeFlags = 0;
		HWPort.TXInterruptEnable = 0;
		HWPort.clearFIFO = 1;
		HWPort.someSettings = 0xFF;
		InterfacePort.Status clearBITS(PORT_SENDING_LAST_BYTE | PORT_SENDING | PORT_BUSY);
		StopTimerWP(&InterfacePort.SendingTimer);
	}
	else {
		res = -4;
	}
	//if (InterfacePort.Status & PORT_SENDING)
		//res = InterfacePort.outCursor;
	return res;
}

__Receive(InterfacePortHandle_t* ifsPort, u16 maxPossibleLen)
{
	int res = 0;
	u8 IsRecvTimerRinging = IsTimerWPRinging(&InterfacePort.ReceivingTimer);
	if (InterfacePort.Status & (PORT_READY | PORT_RECEIVING | PORT_BUSY) == ONLY PORT_READY) {
		InterfacePort.LenDataToRecv = maxPossibleLen;
		LaunchTimerWP(InterfacePort.ReceivingTimer.setVal, &InterfacePort.ReceivingTimer);
		InterfacePort.Status |= PORT_BUSY | PORT_RECEIVING;
		InterfacePort.Status &= ~PORT_RECEIVED;
		HWPort.clearOrResetSomeFlags = 0;
		HWPort.someSettings = 0xFF;
		HWPort.clearFIFO = 1; //?mb
		HWPort.RXInterruptEnable = 1;
		HWPort.StartRX = 1;
	}
	else if ((InterfacePort.Status & (PORT_BUSY | PORT_RECEIVED)) && !IsRecvTimerRinging == ONLY PORT_BUSY | PORT_RECEIVED) {
		//InterfacePort.LenDataToRecv ..
		InterfacePort.inCursor += sizeof(HWPort.FIFO_BUFFER);
		memcpy(InterfacePort.BufferRecved, HWPort.FIFO_BUFFER, sizeof(HWPort.FIFO_BUFFER));
		InterfacePort.Status clearBITS(PORT_RECEIVED);
		//if(InterfacePort.inCursor >= InterfacePort.LenDataToRecv){
		//}
		HWPort.clearFIFO = 1;//?mb
		HWPort.clearOrResetSomeFlags = 0;
		HWPort.RXInterruptEnable = 0;
		HWPort.someSettings = 0xff;
		HWPort.RXInterruptEnable = 1;
		HWPort.StartRX = 1;
	}
	else {
		res = -4;
	}
	if (IsRecvTimerRinging && (InterfacePort.Status & PORT_RECEIVING)) {
		HWPort.clearOrResetSomeFlags = 0;
		HWPort.RXInterruptEnable = 0;
		HWPort.StartRX = 0;
		HWPort.someSettings = 0xff;
		HWPort.clearFIFO = 1;
		//memset(HWPort.FIFO_BUFFER, 0, sizeof(HWPort.FIFO_BUFFER /*.LenDataToRecv*/));
		StopTimerWP(&InterfacePort.ReceivingTimer);
		InterfacePort.Status clearBITS(PORT_RECEIVING | PORT_BUSY);
		InterfacePort.Status |= PORT_RECEIVED; //? mb RECEIVED_TIMEOUT or RECEIVED_ALL?
		InterfacePort.LenDataToRecv = InterfacePort.inCursor;
	}
	return res;
}

___HardwareSentInterrupt()
{

}

___HardwareReceiveInterrupt()
{

}
#endif // IN_CASE_OF_FIFO_TYPE


__HardwareSentInterrupt()
{
	u16 both = 0;
	u16 all = 0;
	if (InterfacePort.Status & (both = (PORT_READY | PORT_SENDING)) == both ONLY) { //? check this!
		InterfacePort.Status |= PORT_SENDED; //?mb it better when flags are being handled in one place? That would be more readable
		_Write(NULL, no_required_now);
	}
}

__HardwareReceiveInterrupt()
{
	u16 both = 0, all = 0;
	if (InterfacePort.Status & (both = (PORT_READY | PORT_RECEIVING)) == both ONLY) {
		InterfacePort.Status |= PORT_RECEIVED;
		_Receive(NULL, no_required_now);
	}
	return;
}

/*
		 _____________
		|     BUS     \
		|___ ______ ___|
		    O      O
*/

__TimerInterrupt() {}; //mb
#endif // !DISABLE_BLACKNOTE
#pragma endregion