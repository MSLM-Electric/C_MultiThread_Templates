#include "CmdsScenarioScripts.h"
//#include "../../Lib/fileHandle.h"

//FIL MutexFileHandle;
//FIL ScanfMutex;

void MakingPacketScenarios(char* buffer, const int maxPossibleLen, void* arg)
{
	UNUSED(arg);
	memset(buffer, 0, maxPossibleLen);
#ifdef MASTER_PORT_PROJECT
	//mutxMasterCfg//!
	memset(&ThisMastersConfigs, 0, sizeof(ThisMastersConfigs));
	printf_s("Enter the SLAVE Address:\n");
	scanf_s("%d", buffer);
	ThisMastersConfigs.SlavesAddressToTalk = *(uint16_t*)&buffer[0];
	printf_s("Enter the function\n");
	scanf_s("%d", buffer);
	ThisMastersConfigs.function = (uint8_t)buffer[0];
	printf_s("Enter the address of SLAVE Memory to talk\n");
	scanf_s("%d", buffer);
	ThisMastersConfigs.AddressOfSlavesMemoryToTalk = *(uint16_t*)&buffer[0];
	printf_s("Enter the length data for talking\n");
	scanf_s("%d", buffer);
	ThisMastersConfigs.LenDataToTalk = *(uint16_t*)&(buffer[0]);
	printf_s("Enter the communication period\n");
	scanf_s("%d", buffer);
	ThisMastersConfigs.communicationPeriod = *(uint16_t*)&buffer[0];
	printf_s("Enter the array of data to read/write\n");
	memset(buffer, 0, maxPossibleLen);
	ScanKeyboardWithWhiteSpaces(buffer, 255);
	memcpy_s(InterfacePort.BufferToSend, 255, buffer, 255);
	ThisMastersConfigs.dataToWrite = InterfacePort.BufferToSend;
	ThisMastersConfigs.Status = 1; //Masters configuration inited!
#elif defined(SLAVE_PORT_PROJECT)
	;
#endif // MASTER_PORT_PROJECT
	ConsolesMenuHandle.CMD[MAKE_PACKET] = 0;
	return;
}

void SetTimerPeriodCmdFunction(char* buffer, const int maxPossibleLen, void* arg)
{
	UNUSED(arg);
	memset(buffer, 0, maxPossibleLen);
	printf_s("Set Monitoring Timers period:\n");
	scanf_s("%d", buffer);
	MonitoringTim.setVal = *(uint16_t*)&(buffer[0]);
	ConsolesMenuHandle.CMD[SET_TIMER_PERIOD] = 0;
	return;
}

void ConfigTracerParams(char* buffer, const int maxPossibleLen, void* arg)
{
	UNUSED(arg);
	memset(buffer, 0, maxPossibleLen);
	printf_s("Config Port trace:\nSet Trace time:");
	scanf_s("%d", buffer);
	if (*(U32_ms*)&(buffer[0]))
		PortTracer.TraceTime.setVal = *(U32_ms*)&(buffer[0]);
	printf_s("Set Trace frequency:\n");
	scanf_s("%d", buffer);
	if(*(U32_ms*)&(buffer[0]))
		PortTracer.FrequencyTrace.setVal = *(U32_ms*)&(buffer[0]);
	ConsolesMenuHandle.CMD[TRACE_CONFIGS] = 0;
	return;
}

#ifdef SLAVE_PORT_PROJECT
void ConfigSlave(char* buffer, const int maxPossibleLen, void* arg)
{
	//mutxSlaveCfg//!
	UNUSED(arg);
	memset(buffer, 0, maxPossibleLen);
	memset(&ThisSlavesConfigs, 0, sizeof(ThisSlavesConfigs));
	printf("Enter this Slaves Address: ");
	scanf_s("%d", buffer);
	ThisSlavesConfigs.MyAddress = *(uint16_t*)&buffer[0];
	printf("Enter the Response timeout from this Slave: ");
	scanf_s("%d", buffer);
	printf("\n");
	if (*(uint16_t*)&buffer[0])
		ThisSlavesConfigs.ResponseTimeout = *(uint16_t*)&buffer[0];
	ThisSlavesConfigs.dataFromRead = InterfacePort.BufferRecved;
	ThisSlavesConfigs.Status = 1;
	ConsolesMenuHandle.CMD[SLAVE_CFG] = 0;
}
#endif //!SLAVE_PORT_PROJECT

void CommonConfigurate(char* buffer, const int maxPossibleLen, void* arg)
{
	//TakeGLOBMutex(&ScanfMutex, 0xFFFF);
#ifdef MASTER_PORT_PROJECT
	MakingPacketScenarios(buffer, maxPossibleLen, arg);
#elif SLAVE_PORT_PROJECT
	ConfigSlave(buffer, maxPossibleLen, arg);
#endif // MASTER_PORT_PROJECT
	ConfigTracerParams(buffer, maxPossibleLen, arg);
	ConsolesMenuHandle.CMD[COMMON_CONFIGS] = 0;
	//RealeaseGLOBMutex(&ScanfMutex);
}