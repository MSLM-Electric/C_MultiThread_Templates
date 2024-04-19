#include "HardwareInterfaceUnit.h"
#include "../IO_immitationBetweenMasterSlave/PortsBusMessages.h"
#include "../../MultiThreadSupport.h"

extern char mastersMessageId[] = "MASTER_WRITE:";
HANDLE iofileMutex;
FIL FileHandle;

#define ONLY //just nothing. Only for clarifying ports state currently

int InitPort(InterfacePortHandle_t* PortHandle)
{
	int res = 0;
	iofileMutex = CreateMutexW(NULL, 1, "IOFILE_Mutex");
	ReleaseMutex(iofileMutex);
	return res;
}

int Write(InterfacePortHandle_t* PortHandle, const uint8_t *inBuff, const int size)
{
	int res;
	if ((PortHandle->Status & (PORT_READY | PORT_SENDING | PORT_RECEIVING)) == ONLY PORT_READY) {
		PortHandle->Status |= PORT_SENDING;
		//memcpy(PortHandle->BufferToSend, inBuff, size);
		res = immitationOfPortsBus(PortHandle);
		if (res < 0) {
			//PORT_ERROR//?;
		}
	}
}

int Recv(InterfacePortHandle_t* PortHandle, uint8_t *outBuff, const int maxPossibleSize)
{

}

static int immitationOfPortsBus(InterfacePortHandle_t* PortHandle) //! immitationSendingOfPortsBus()
{
	int res = 0;
	char buffer[300];
	//char portsBusMessageId = portsMessageId;
	char *mastersBusMessageId = mastersMessageId;
	//"%s%s %s\n"
	sprintf(buffer, "%s %s\n", mastersBusMessageId, PortHandle->BufferToSend); //sizes?
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
	ReleaseMutex(iofileMutex);
	res = (int)siz;
	fclose(f);
	if (res > 0) {
		ThisMastersConfigs/*[PortNo]*/.currentIOfileLine++;
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
	if ((fres == FR_OK) && (strncmp(buffer, slavesMessageId, strlen(slavesMessageId)) == 0)) { //fres = FR_INVALID_PARAMETER?? But is ok?

		if (ThisMastersConfigs.lastReadedLine != ThisMastersConfigs.currentIOfileLine) {
			//Port occured a data on Bus 
			//we pretend that the Hardware has the big FIFO
			memcpy_s(InterfacePort.BufferRecved, sizeof(InterfacePort.LenDataToRecv), &buffer[strlen(slavesMessageId)], sizeof(InterfacePort.LenDataToRecv));
			ThisMastersConfigs.lastReadedLine = ThisMastersConfigs.currentIOfileLine++;
			Called_RXInterrupt(&InterfacePort);
		}
	}
	else {
		res = (int)fres;
	}
	return res;
}

void TransmitInterrupt(void *arg) 
{
	InterfacePortHandle_t* Port = (InterfacePortHandle_t *)arg;
	Port->Status &= ~PORT_SENDING;
	Port->Status |= PORT_SENDED;
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