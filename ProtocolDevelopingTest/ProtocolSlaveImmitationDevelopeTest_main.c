#define _CRT_SECURE_NO_WARNINGS  //!!! to allow unsafe and oldest code styles

#include "../MultiThreadSupport.h"
#include "../Lib/SimpleTimerWP.h"
#include "../ConsoleMenuOptions.h"
#include "HardwarePeripheral/HardwareInterfaceUnit.h"
#include "../Lib/type_def.h"

#define false 0
#define true 1

uint8_t someData[128] = {0};
uint8_t getData[1024];
uint16_t catchPoint = 0;

/*DE*/ uint8_t MoreDetailsInShowing = 0;
/*PA*/ uint8_t PauseConsoleCommand = 0;

enum {
	INTERRUPT_CALLED = 1,
	NOT_INTERRUPT_STATE = 2
}State_t;

enum {
	DMA_READY = 1,
	DMA_BUSY = 2
}dmastate_t;

typedef struct{
	uint8_t State;
}interrupt_simulate_handle_t, dma_simulate_handle_t;

interrupt_simulate_handle_t callInterrupt;
dma_simulate_handle_t DMAHandle;
Timerwp_t UsersTimer;
Timerwp_t MonitoringTim;
/*TI*/ uint8_t testTimer = 0;
InterfacePortHandle_t InterfacePort;

static void MakingPacketScenarios(char* buffer, const int maxPossibleLen, void* arg);
static void SetTimerPeriodCmdFunction(char* buffer, const int maxPossibleLen, void* arg);
static void RegisterCmdFunctionsCallback(void);

void callback()
{
	//Users code here
	return;
}

void TimerCallback() //InterruptTimer
{
	//Useres code here
	return;
}

DWORD WINAPI ThreadNo1(LPVOID lpParam);
DWORD WINAPI ThreadNo2(LPVOID lpParam);
DWORD WINAPI TickThread(LPVOID lpParam);
ThreadsStruct_t Thread1Struct;
ThreadsStruct_t Thread2Struct;
ThreadsStruct_t TickThreadStruct;

HANDLE sem;
HANDLE mutx;
char keyboardBuff[20];
uint8_t scanfIsBusy = 0;

int main()
{
	sem = CreateSemaphoreW(NULL, 3, 3, "NT5BBSEM");
	mutx = CreateMutexW(NULL, 1, "Mutex");

	int res = 0;
	res = ThreadCreation(&ThreadNo1, &Thread1Struct, 1);
	res = ThreadCreation(&ThreadNo2, &Thread2Struct, 2);
	res = ThreadCreation(&TickThread, &TickThreadStruct, 4);

	// Aray to store thread handles 
	HANDLE Array_Of_Thread_Handles[4];
	// Store Thread handles in Array of Thread
	// Handles as per the requirement
	// of WaitForMultipleObjects() 
	Array_Of_Thread_Handles[0] = Thread1Struct.Handle_Of_Thread;
	Array_Of_Thread_Handles[1] = Thread2Struct.Handle_Of_Thread;
	Array_Of_Thread_Handles[3] = TickThreadStruct.Handle_Of_Thread;

	// Wait until all threads have terminated.
	WaitForMultipleObjects(3, Array_Of_Thread_Handles, TRUE, INFINITE);

	memset(someData, 0, sizeof(someData));
	ReleaseMutex(mutx);  //free mutex to start program
	Timerwp_t MainProgrammDelay;
#ifdef DEBUG_ON_VS
	InitTimerWP(&UsersTimer, (tickptr_fn*)GetTickCount);
	InitTimerWP(&MainProgrammDelay, (tickptr_fn*)GetTickCount);
#endif // DEBUG_ON_VS
	LaunchTimerWP((U32_ms)2000, &MainProgrammDelay);
	RegisterCmdFunctionsCallback();
	ConsolesMenuHandle.CMD[PAUSE_CONSOLE] = 1;

	while (1)
	{
		PauseConsoleCommand = ConsolesMenuHandle.CMD[PAUSE_CONSOLE];
		if (!PauseConsoleCommand) {
			if (IsTimerWPRinging(&MainProgrammDelay))//Sleep(2000);
			{
				RestartTimerWP(&MainProgrammDelay);
				printf("MainBckgdProccess\n");
			}
		}
		if (ConsolesMenuHandle.CMD[START_COMMUNICATION]) {
			;
		}
	}
	printf("endOfCycle. Bad jump! \n"); //programm execution never should get here!
}

DWORD WINAPI ThreadNo1(LPVOID lpParam)
{
	int res = ThreadInit(lpParam);

	//char *keyboardBuff = (char *)malloc(20 * sizeof(char));
	char keyboardBuff[255] = {0};

	while (1)
	{
		WaitForSingleObject(mutx, INFINITE);
		{
			memset(keyboardBuff, 0, sizeof(keyboardBuff));
			printf("What function to Act? Enter it here: ");
			scanf_s("%s", keyboardBuff, 255);
			printf("entered data is: %s\n", keyboardBuff);
		}
		ReleaseMutex(mutx);
		if (keyboardBuff == NULL)
		{
			printf("Memory for str alloc ERROR\t\n");
		}
		SettingsCMD_Handling(keyboardBuff, NULL);
		WaitForSingleObject(mutx, INFINITE);
		ScanCMDsScenarios(keyboardBuff, sizeof(keyboardBuff));
		ReleaseMutex(mutx);
	}
}

#pragma region DMA_IMMITATION_BY_BUTTON
DWORD WINAPI ThreadNo2(LPVOID lpParam)
{
	int res = ThreadInit(lpParam);

	uint32_t buttonForCallInterruptStateChange = 2;
	while (1)
	{
		//Sleep(10);
		WaitForSingleObject(mutx, INFINITE);
		{
			if (ConsolesMenuHandle.CMD[DMA_ENABLE]) {
				printf_s("Enter The interrupt calling state:\n");
				scanf_s("%d", &buttonForCallInterruptStateChange);
				printf("entered data is: %d\n", buttonForCallInterruptStateChange);
			}
		}
		ReleaseMutex(mutx);
		if (buttonForCallInterruptStateChange == 1) {
			callInterrupt.State = INTERRUPT_CALLED;
			callback();
			callInterrupt.State = NOT_INTERRUPT_STATE;
		}
		if (buttonForCallInterruptStateChange == 2)
			callInterrupt.State = NOT_INTERRUPT_STATE;
		buttonForCallInterruptStateChange = 0;
	}
}
#pragma endregion

DWORD WINAPI TickThread(LPVOID lpParam)
{
	int res = ThreadInit(lpParam);

	uint16_t testCount = 0;
	while (1)
	{
		if (ConsolesMenuHandle.CMD[ENABLE_TIMER]) {
			LaunchTimerWP(MonitoringTim.setVal, &MonitoringTim);
			if (IsTimerWPRinging(&MonitoringTim))
				RestartTimerWP(&MonitoringTim);
		}else {
			StopTimerWP(&MonitoringTim);
		}
#define BLACKNOTE_MAIN
#ifdef BLACKNOTE_MAIN
		if (IsTimerWPRinging(&InterfacePort.SendingTimer)) {
			//.. Do sending case works
			//..
			StopTimerWP(&InterfacePort.SendingTimer);
			//DelayedRecv //?
		}
#endif // BLACK_NOTE_MAIN

	}
}

static void MakingPacketScenarios(char* buffer, const int maxPossibleLen, void* arg)
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
	//mutxSlaveCfg//!
	memset(&ThisSlavesConfigs, 0, sizeof(ThisSlavesConfigs));
	printf("Enter this Slaves Address: ");
	scanf_s("%d", buffer);
	ThisSlavesConfigs.MyAddress = *(uint16_t*)&buffer[0];
	printf("Enter the Response timeout from this Slave: ");
	scanf_s("%d", buffer);
	printf("\n");
	ThisSlavesConfigs.ResponseTimeout = *(uint16_t*)&buffer[0];
	ThisSlavesConfigs.dataFromRead = InterfacePort.BufferRecved;
	ThisSlavesConfigs.Status = 1;
#endif // MASTER_PORT_PROJECT
	ConsolesMenuHandle.CMD[MAKE_PACKET] = 0;
	return;
}

static void SetTimerPeriodCmdFunction(char* buffer, const int maxPossibleLen, void* arg)
{
	UNUSED(arg);
	memset(buffer, 0, maxPossibleLen);
	printf_s("Set Monitoring Timers period:\n");
	scanf_s("%d", buffer);
	MonitoringTim.setVal = *(uint16_t*)&(buffer[0]);
	ConsolesMenuHandle.CMD[SET_TIMER_PERIOD] = 0;
	return;
}

static void RegisterCmdFunctionsCallback(void)
{
	ConsolesMenuHandle.executeFunc[MAKE_PACKET] = (callback_fn*)MakingPacketScenarios;
	ConsolesMenuHandle.executeFunc[SET_TIMER_PERIOD] = (callback_fn*)SetTimerPeriodCmdFunction;
}