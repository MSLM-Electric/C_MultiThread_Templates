#define _CRT_SECURE_NO_WARNINGS  //!!! to allow unsafe and oldest code styles

#include "HardwarePeripheral/HardwareInterfaceUnit.h"
#include "../MultiThreadSupport.h"
#include "../Lib/SimpleTimerWP.h"
#include "../ConsoleMenuOptions.h"
#include "IO_immitationBetweenMasterSlave/MasterImmitationCfg.h"

#define false 0
#define true 1

void SettingsCMD_Handling(char* inBuff, const uint16_t maxPossibleLen);
int ScanKeyboardWithWhiteSpaces(char* inBuff, uint16_t maxPossibleLen);
thisMastercfgs_t MasterInterface;

uint8_t someData[128] = {0};
uint8_t getData[1024];
uint16_t catchPoint = 0;
extern char iofilePath[] = IOFILE_PATH;

/*DE*/ uint8_t MoreDetailsInShowing = 0;
/*PA*/ uint8_t PauseConsoleCommand = 1;

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
/*TI*/ uint8_t testTimer = 0;

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

DWORD WINAPI ThreadWriting(LPVOID lpParam);
DWORD WINAPI ThreadNo2(LPVOID lpParam);
DWORD WINAPI TickThread(LPVOID lpParam);
DWORD WINAPI ThreadReading(LPVOID lParam);
ThreadsStruct_t Thread1Struct;
ThreadsStruct_t Thread2Struct;
ThreadsStruct_t TickThreadStruct;
ThreadsStruct_t ThreadReadingStruct;

static void ShowAllStates(void);
HANDLE sem;
HANDLE mutx;
char keyboardBuff[20];
uint8_t scanfIsBusy = 0;


int main()
{
	sem = CreateSemaphoreW(NULL, 3, 3, "NT5BBSEM");
	mutx = CreateMutexW(NULL, 1, "Mutex");

	int res = 0;
	res = ThreadCreation(&ThreadWriting, &Thread1Struct, 1);
	res = ThreadCreation(&ThreadNo2, &Thread2Struct, 2);
	res = ThreadCreation(&TickThread, &TickThreadStruct, 4);
	res = ThreadCreation(&ThreadReading, &ThreadReadingStruct, 5);

	// Aray to store thread handles 
	HANDLE Array_Of_Thread_Handles[5];
	// Store Thread handles in Array of Thread
	// Handles as per the requirement
	// of WaitForMultipleObjects() 
	Array_Of_Thread_Handles[0] = Thread1Struct.Handle_Of_Thread;
	Array_Of_Thread_Handles[1] = Thread2Struct.Handle_Of_Thread;
	Array_Of_Thread_Handles[3] = TickThreadStruct.Handle_Of_Thread;
	Array_Of_Thread_Handles[4] = ThreadReadingStruct.Handle_Of_Thread;

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
	InitPort(&InterfacePort);

	while (1)
	{
		if (ConsolesMenuHandle.CMD[PAUSE_CONSOLE]) {
			PauseConsoleCommand = ~PauseConsoleCommand & 0x1;
			ConsolesMenuHandle.CMD[PAUSE_CONSOLE] = 0; //Do once by entering command
		}
		if (!PauseConsoleCommand) {
			if (IsTimerWPRinging(&MainProgrammDelay))//Sleep(2000);
			{
				RestartTimerWP(&MainProgrammDelay);
				printf("MainBckgdProccess\n");
			}
		}
		if (testTimer) {;}
		else {	;/*stoptimer*/ }

		if (ConsolesMenuHandle.CMD[START_COMMUNICATION] && ThisMastersConfigs.Status) { //mutxMaster//!
			//if (InterfacePort.Status & 0x2)
				InterfacePort.Status |= PORT_READY;
			InterfacePort.communicationPeriod = ThisMastersConfigs.communicationPeriod;
			//InterfacePort.Status = ThisMastersConfigs.Status; //Port ready to communicating
			InterfacePort.LenDataToSend = ThisMastersConfigs.LenDataToTalk;
		}else {
			InterfacePort.Status &= ~PORT_READY;
		}

		/*Users Thread. */
		//SendRequest(PortNo=0, SlaveAddr=1, func=3, itsMemoryAddr=0x020, qntyData=10, communPeriod=400, *); 
		if (InterfacePort.Status) {
			LaunchTimerWP(InterfacePort.communicationPeriod, &UsersTimer);
			if (IsTimerWPRinging(&UsersTimer)) {
				RestartTimerWP(&UsersTimer);
				int res = Write(&InterfacePort, &InterfacePort.BufferToSend, InterfacePort.LenDataToSend);
				res = Recv(&InterfacePort, &InterfacePort.BufferRecved, sizeof(InterfacePort.BufferRecved));
			}
		}
	}
	printf("endOfCycle. Bad jump! \n"); //programm execution never should get here!
}

//ThreadNo1
DWORD WINAPI ThreadWriting(LPVOID lpParam)//rename to ThreadConsoleHandling or ThreadCMDsHandling
{
	int res = ThreadInit(lpParam);

	char *str = (char *)malloc(4);
	//char *keyboardBuff = (char *)malloc(20 * sizeof(char));
	char keyboardBuff[255];
	while (1)
	{
		WaitForSingleObject(mutx, INFINITE);
		{
			memset(keyboardBuff, 0, sizeof(keyboardBuff));
			printf("What function to Act? Enter it here:\n");
			scanf_s("%s", keyboardBuff/*&str*/, 255);
			printf("entered data is: %s\n", keyboardBuff);
		}
		ReleaseMutex(mutx);
		if (str == NULL)
		{
			printf("Memory for str alloc ERROR\t\n");
		}else {
			sprintf(str, keyboardBuff, 255);
			memset(keyboardBuff, 0, sizeof(keyboardBuff));
		}
		SettingsCMD_Handling(str, NULL);
		WaitForSingleObject(mutx, INFINITE);
		ScanCMDsScenarios(keyboardBuff, sizeof(keyboardBuff));
		ReleaseMutex(mutx);
	}
}

DWORD WINAPI ThreadNo2(LPVOID lpParam)
{
	int res = ThreadInit(lpParam);

	uint8_t buttonForCallInterruptStateChange = 2;
	char keyboardBuff[255];
	while (1)
	{
		//Sleep(10);
		WaitForSingleObject(mutx, INFINITE);
		{
			if (ConsolesMenuHandle.CMD[DMA_ENABLE]) {
				printf_s("Enter The interrupt calling state:\n");
				scanf_s("%d", &buttonForCallInterruptStateChange);
				printf("entered the interrupt state data is: %d\n", buttonForCallInterruptStateChange);
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

DWORD WINAPI TickThread(LPVOID lpParam)
{
	int res = ThreadInit(lpParam);

	uint16_t testCount = 0;
	while (1)
	{
	}
}

DWORD WINAPI ThreadReading(LPVOID lpParam)
{
	int res = ThreadInit(lpParam);

	uint16_t testCountR = 0;
	//InterfacePortHandle_t Port;
	Timerwp_t readingIOfilePeriod;
	InitTimerWP(&readingIOfilePeriod, (tickptr_fn*)GetTickCount);
	LaunchTimerWP((U32_ms)1000, &readingIOfilePeriod);
	while (1)
	{
		if (IsTimerWPRinging(&readingIOfilePeriod)) {
			RestartTimerWP(&readingIOfilePeriod);
			if(InterfacePort.Status & (PORT_READY | PORT_RECEIVING) == (PORT_READY | PORT_RECEIVING))
				immitationReceivingOfPortsBus(&InterfacePort);  //reading file shouldn't be so fast!
		}
	}
}