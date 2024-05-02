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
	char buffer[300];
	int PortNo = 0;
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
	FRESULT fres = ReadTheLastLineOfFile(buffer, sizeof(buffer), f);
	ReleaseMutex(iofileMutex);
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
			if (ThisMastersConfigs.lastReadedLine != ThisMastersConfigs.currentIOfileLine) {
				//Port detected a datas on Bus 
				//we pretend that the Hardware has the big FIFO
				memcpy_s(InterfacePort.BufferRecved, sizeof(InterfacePort.LenDataToRecv), &buffer[strlen(slavesMessageId)], sizeof(InterfacePort.LenDataToRecv));
				ThisMastersConfigs.lastReadedLine = ThisMastersConfigs.currentIOfileLine++;
				//Called_RXInterrupt(&InterfacePort);
			}
		}
		else if ((DirectionSendingOfBusMessageId == mastersMessageId)) {
			;
		}
		Called_RXInterrupt(&InterfacePort);
	}
	else { //?
		res = (int)fres;
		RestartTimerWP(&outPortHandle->ReceivingTimer);
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

void Called_RXInterrupt(void* arg)
{
	InterfacePortHandle_t* Port = (InterfacePortHandle_t*)arg;
	Port->Status |= PORT_RECEIVED;
	if (InterfacePort.BufferRecved[0] == ThisMastersConfigs.SlavesAddressToTalk){ //IsThisDataForMe();
		;
	}
	return;
}