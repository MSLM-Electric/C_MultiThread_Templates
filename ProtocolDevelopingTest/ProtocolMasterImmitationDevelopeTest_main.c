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
ThreadsStruct_t Thread1Struct;
ThreadsStruct_t Thread2Struct;
ThreadsStruct_t TickThreadStruct;

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

	while (1)
	{
		if (!PauseConsoleCommand) {
			if (IsTimerWPRinging(&MainProgrammDelay))//Sleep(2000);
			{
				RestartTimerWP(&MainProgrammDelay);
				printf("MainBckgdProccess\n");
			}
		}
		if (testTimer) {;}
		else {	;/*stoptimer*/ }


	}
	printf("endOfCycle. Bad jump! \n"); //programm execution never should get here!
}

//ThreadNo1
DWORD WINAPI ThreadWriting(LPVOID lpParam)
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
	}
}

void SettingsCMD_Handling(char* inBuff, const uint16_t maxPossibleLen)
{
	uint8_t parsedCMD = StringCompareAndParseToNum(inBuff, NULL);
	ConsolesMenuHandle.CMDcontrol[parsedCMD] = ~ConsolesMenuHandle.CMDcontrol[parsedCMD] & 0x01;
	uint8_t IsParsedCMD_Enabled = ConsolesMenuHandle.CMDcontrol[parsedCMD];
	switch (parsedCMD)
	{
		//Users code
		/*------------------------------Put your Functions launch here----------------------------*/
		//case EXAMPLE: {
		//	ExampleOfYourFunctions();
		//}break;
		/*----------------------------------------------------------------------------------------*/
	case ALL: {
		ShowAllStates();
	}break;
	case DETAILS: {
		if (IsParsedCMD_Enabled)
			printf("DETAILS ON!\n");
		else
			printf("DETAILS OFF!\n");
	}break;
	case PAUSE_CONSOLE: {
		if (IsParsedCMD_Enabled)
			printf("Pause Console ON: Mainbackground process don't show!\n");
		else
			printf("Pause Console OFF: Show Mainbackground process!\n");
	}break;
	case ENABLE_TIMER: {
		if (IsParsedCMD_Enabled)
			printf("Timer ENABLED!\n");
		else
			printf("Timer DISABLED!\n");
	}break;
	case MAKE_PACKET: {

	}break;
	case DMA_ENABLE: {
		if (IsParsedCMD_Enabled)
			printf("DMA interrupting peripheral ON!\n");
		else
			printf("DMA interrupting peripheral OFF!\n");
	}break;
	case START_COMMUNICATION: {
		if (IsParsedCMD_Enabled) {
			printf("START COMMUNICATION\n");
			ConsolesMenuHandle.CMDcontrol[STOP_COMMUNICATION] = 0;
		}
	}break;
	case STOP_COMMUNICATION: {
		if (IsParsedCMD_Enabled) {
			printf("STOP COMMUNICATION\n");
			ConsolesMenuHandle.CMDcontrol[START_COMMUNICATION] = 0;
		}
	}break;
	default:
		break;
	}
	memset(inBuff, 0, 2); //memsetstr
	return;
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
			if (ConsolesMenuHandle.CMDcontrol[DMA_ENABLE]) {
				printf_s("Enter The interrupt calling state:\n");
				scanf_s("%d", &buttonForCallInterruptStateChange);
				printf("entered the interrupt state data is: %d\n", buttonForCallInterruptStateChange);
			}
			if (ConsolesMenuHandle.CMDcontrol[MAKE_PACKET]) {
				memset(keyboardBuff, 0, sizeof(keyboardBuff));
				memset(&ThisMastersConfigs, 0, sizeof(ThisMastersConfigs));
				printf_s("Enter the SLAVE Address:\n");
				scanf_s("%d", keyboardBuff);
				ThisMastersConfigs.SlavesAddressToTalk = *(uint16_t*)&keyboardBuff[0];
				printf_s("Enter the function\n");
				scanf_s("%d", keyboardBuff);
				ThisMastersConfigs.function = (uint8_t)keyboardBuff[0];
				printf_s("Enter the address of SLAVE Memory to talk\n");
				scanf_s("%d", keyboardBuff);
				ThisMastersConfigs.AddressOfSlavesMemoryToTalk = *(uint16_t*)&keyboardBuff[0];
				printf_s("Enter the length data for talking\n");
				scanf_s("%d", keyboardBuff);
				ThisMastersConfigs.LenDataToTalk = *(uint16_t *)&(keyboardBuff[0]);
				printf_s("Enter the communication period\n");
				scanf_s("%d", keyboardBuff);
				ThisMastersConfigs.communicationPeriod = *(uint16_t*)&keyboardBuff[0];
				printf_s("Enter the array of data to read/write\n");
				memset(keyboardBuff, 0, sizeof(keyboardBuff));
				ScanKeyboardWithWhiteSpaces(keyboardBuff, 255);
				memcpy_s(InterfacePort.BufferToSend, 255, keyboardBuff, 255);
				ThisMastersConfigs.dataToWrite = InterfacePort.BufferToSend;
				ConsolesMenuHandle.CMDcontrol[MAKE_PACKET] = 0;
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

int ScanKeyboardWithWhiteSpaces(char* inBuff, uint16_t maxPossibleLen)
{
	int res = -1;
	uint16_t ptr = 0;
	char catchedChar = 0;
	memset(inBuff, 0, maxPossibleLen);
	uint8_t timeout = 200;
	do {
		scanf_s("%s%c", (char *)&inBuff[ptr], 255, &catchedChar, 1);
		ptr = strlen(inBuff);
		inBuff[ptr] = ' ';
		ptr++;
	} while ((catchedChar != '\n') && (timeout--)); //(catchedChar == ' ')
	if (timeout == 0)
		return res;
	inBuff[ptr] = '\0';
	return res = 0;
}

DWORD WINAPI TickThread(LPVOID lpParam)
{
	int res = ThreadInit(lpParam);

	uint16_t testCount = 0;
	while (1)
	{
	}
}


static void ShowAllStates(void)
{
#ifdef DEBUG /*|| ENABLE_SOME_STATES_MONITORING*/
	printf("\n\n\n..............\n");
	printf("State 1: = %d\n", State1);
	printf("State 2: = %d\n", State2);
	printf("State 3: = %d\n", State3);
	printf("..............\n");
#define MORE_DETAILS_SHOW 1
#if (MORE_DETAILS_SHOW == 1)
	if (MoreDetailsInShowing) {
		if (State1.Status & OPEN) {
			printf("+--> State1.member: = %d\n", State1.member);
		}
		if (State2.Status & OPEN) {
			printf("+--> State2.member: = %d\n", State2.member);
		}
		if (State3.Status & OPEN) {
			printf("+--> State3 - Status: = %d\n", State3.Status);
			printf("+--> State3 - member: = %d\n", State3.member);
		}
	}	
#endif
#endif
	return;
}