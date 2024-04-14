#include "HardwareInterfaceUnit.h"
extern char mastersMessageId[] = "MASTER_WRITE:";

int Write(InterfacePortHandle_t* PortHandle, const uint8_t *inBuff, const int size)
{
	if ((PortHandle->Status & (PORT_READY | PORT_SENDING | PORT_RECEIVING)) == PORT_READY) {
		PortHandle->Status |= PORT_SENDING;
		//memcpy(PortHandle->BufferToSend, inBuff, size);
		immitationOfPortsBus(PortHandle);
	}
}

int Recv(InterfacePortHandle_t* PortHandle, uint8_t *outBuff, const int maxPossibleSize)
{

}

static int immitationOfPortsBus(InterfacePortHandle_t* PortHandle)
{
	int res = 0;
	char buffer[300];
	char *mastersBusMessageId = mastersMessageId;
	sprintf(buffer, "%s %s\n", mastersBusMessageId, PortHandle->BufferToSend); //sizes?
	FIL* f = fopen(iofilePath, "a+"); //a
	size_t siz = fwrite(buffer, strlen(buffer), 1/*??*/, f);// fwprintf, .._s
	res = (int)siz;
	fclose(f);
	if (res > 0)
		TransmitInterrupt(PortHandle); //Call_TXInterrupt()
	return res;
}

void TransmitInterrupt(void *arg) {
	InterfacePortHandle_t* Port = (InterfacePortHandle_t *)arg;
	Port->Status &= ~PORT_SENDING;
	Port->Status |= PORT_SENDED;
}