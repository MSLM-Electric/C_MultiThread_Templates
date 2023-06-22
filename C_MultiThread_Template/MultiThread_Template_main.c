#define _CRT_SECURE_NO_WARNINGS  //!!! to allow unsafe and oldest code styles

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
//#include <windows.system.threading.h>
//#include <processthreadsapi.h>
#include <Windows.h>
#include <strsafe.h>

#define false 0
#define true 1

uint8_t someData[128] = {
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0
};

uint32_t globTick;
uint32_t timerVal = 0;
struct {
	uint32_t setVal;
	uint32_t launchedTime;
	uint8_t Start;
}Timer;

uint8_t getData[1024];
uint16_t catchPoint = 0;

/*DE*/ uint8_t MoreDetailsInShowing = 0;
/*PA*/ uint8_t PauseConsoleCommand = 0;

enum {
	INTERRUPT_CALLED = 1,
	NOT_INTERRUPT_STATE = 2
}State_t;

typedef struct{
	uint8_t State;
}interrupt_simulate_handle_t;

interrupt_simulate_handle_t callInterrupt;

void callback()
{
	return;
}

void TimerCallback() //InterruptTimer
{
	return;
}

extern void LaunchTimer(uint32_t time)
{
	if (Timer.Start == 0)
	{
		Timer.setVal = time;
		Timer.launchedTime = globTick;
	}	
	Timer.Start = 1;
}

void StopTimer()
{
	Timer.setVal = 0;
	Timer.launchedTime = 0;
	Timer.Start = 0;
}

DWORD WINAPI ThreadNo1(LPVOID lpParam);
DWORD WINAPI ThreadNo2(LPVOID lpParam);
DWORD WINAPI TickThread(LPVOID lpParam);
static uint8_t StringCompareAndParseToNum(char* inBuff, uint8_t maxPossibleLen);
static void ShowAllStates(void);
HANDLE sem;
HANDLE mutx;
char keyboardBuff[20];
uint8_t scanfIsBusy = 0;

int main()
{
	sem = CreateSemaphoreW(NULL, 3, 3, "NT5BBSEM");
	mutx = CreateMutexW(NULL, 1, "Mutex");

	// Data of Thread 1
	int Data_Of_Thread_1 = 1;
	// Data of Thread 2
	int Data_Of_Thread_2 = 2;
	// Data of Thread 3
	int Data_Of_Thread_3 = 3;
	// Data of TickThread
	int Data_Of_TickThread = 4;
	// variable to hold handle of Thread 1
	HANDLE Handle_Of_Thread_1 = 0;
	// variable to hold handle of Thread 1 
	HANDLE Handle_Of_Thread_2 = 0;
	// variable to hold handle of Thread 1
	HANDLE Handle_Of_Thread_3 = 0;
	//
	HANDLE Handle_Of_TickThread = 0;
	// Aray to store thread handles 
	HANDLE Array_Of_Thread_Handles[4];

	// Create thread 1.
	Handle_Of_Thread_1 = CreateThread(NULL, 0,
		ThreadNo1, &Data_Of_Thread_1, 0, NULL);
	if (Handle_Of_Thread_1 == NULL)
		ExitProcess(Data_Of_Thread_1);

	// Create thread 2.
	Handle_Of_Thread_2 = CreateThread(NULL, 0,
		ThreadNo2, &Data_Of_Thread_2, 0, NULL);
	if (Handle_Of_Thread_2 == NULL)
		ExitProcess(Data_Of_Thread_2);

	// Create TickThread
	Handle_Of_TickThread = CreateThread(NULL, 0,
		TickThread, &Data_Of_TickThread, 0, NULL);
	if (Handle_Of_TickThread == NULL)
		ExitProcess(Data_Of_TickThread);

	// Store Thread handles in Array of Thread
	// Handles as per the requirement
	// of WaitForMultipleObjects() 
	Array_Of_Thread_Handles[0] = Handle_Of_Thread_1;
	Array_Of_Thread_Handles[1] = Handle_Of_Thread_2;
	Array_Of_Thread_Handles[3] = Handle_Of_TickThread;

	// Wait until all threads have terminated.
	WaitForMultipleObjects(3, Array_Of_Thread_Handles, TRUE, INFINITE);
	uint8_t testFuncsOnDebug = 0;
	ReleaseMutex(mutx);  //free mutex to start program
	while (1)
	{
		if (!PauseConsoleCommand) {
			printf("MainBckgdProccess\n");
			Sleep(2000);
			if (testFuncsOnDebug)
			{
				;
			}
		}
	}
	printf("endOfCycle \n");
}

enum cmdsValEnums{
	ALL = 1,  //it shows all states of OneWire functns
	DETAILS,
	PAUSE_CONSOLE/*,
	EXAMPLE //Users code*/
};


DWORD WINAPI ThreadNo1(LPVOID lpParam)
{
	int     Data = 0;
	char a;
	HANDLE  hStdout = NULL;

	// Get Handle To screen. Else how will we print?
	if ((hStdout = GetStdHandle(STD_OUTPUT_HANDLE)) == INVALID_HANDLE_VALUE)
		return 1;
	// Cast the parameter to the correct
	// data type passed by callee i.e main() in our case.
	Data = *((int*)lpParam);

	char *str = (char *)malloc(4);
	//char *keyboardBuff = (char *)malloc(20 * sizeof(char));
	//char keyboardBuff[20];
	while (1)
	{
		WaitForSingleObject(mutx, INFINITE);
		{
			memset(keyboardBuff, 0, sizeof(keyboardBuff));
			printf("What function to Act? Enter it here:\n");
			scanf_s("%s", keyboardBuff/*&str*/, 3);
			printf("entered data is: %s\n", keyboardBuff);
		}
		ReleaseMutex(mutx);
		if (str == NULL)
		{
			printf("Memory for str alloc ERROR\t\n");
		}
		else {
			sprintf(str, keyboardBuff, 2);
		}memset(keyboardBuff, 0, sizeof(keyboardBuff));
		
		switch (StringCompareAndParseToNum(str, NULL)) //maybe we need do it in another way
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
			MoreDetailsInShowing = ~MoreDetailsInShowing & 0x01;
			if (MoreDetailsInShowing)
				printf("DETAILS ON!\n");
			else
				printf("DETAILS OFF!\n");
		}break;
		case PAUSE_CONSOLE: {
			PauseConsoleCommand = ~PauseConsoleCommand & 0x01;
			if (PauseConsoleCommand)
				printf("Pause Console ON: Mainbackground process don't show!\n");
			else
				printf("Pause Console OFF: Show Mainbackground process!\n");
		}break;
		default:
			break;
		}
		memset(str, 0, 2); //memsetstr
	}
}



DWORD WINAPI ThreadNo2(LPVOID lpParam)
{
	int     Data = 0;
	char a;
	HANDLE  hStdout = NULL;

	// Get Handle To screen. Else how will we print?
	if ((hStdout = GetStdHandle(STD_OUTPUT_HANDLE)) == INVALID_HANDLE_VALUE)
		return 1;
	// Cast the parameter to the correct
	// data type passed by callee i.e main() in our case.
	Data = *((int*)lpParam);

	uint8_t buttonForCallInterruptStateChange = 2;
	while (1)
	{
		//Sleep(10);
		WaitForSingleObject(mutx, INFINITE);
		{
			printf_s("Enter The interrupt calling state:\n");
			scanf_s("%d", &buttonForCallInterruptStateChange);
			printf("entered data is: %d\n", buttonForCallInterruptStateChange);
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
	int     Data = 0;
	char a;
	HANDLE  hStdout = NULL;

	// Get Handle To screen. Else how will we print?
	if ((hStdout = GetStdHandle(STD_OUTPUT_HANDLE)) == INVALID_HANDLE_VALUE)
		return 1;
	// Cast the parameter to the correct
	// data type passed by callee i.e main() in our case.
	Data = *((int*)lpParam);
	Timer.Start = 0;
	while (1)
	{
		Sleep(1);
		globTick++;
		if (((globTick - Timer.launchedTime) > Timer.setVal) * Timer.Start)
		{
			StopTimer();
			TimerCallback();
		}

		/*Catch errors & doubt condition values*/
		if (false == 0) {
			catchPoint = 1;
		}
	}
}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file


static uint8_t StringCompareAndParseToNum(char* inBuff, uint8_t maxPossibleLen)
{
	uint8_t len = maxPossibleLen;
	if (maxPossibleLen == NULL)
	{
		len = 2;
	}

	//Users code
	/*------------------------------Put your Functions launch here----------------------------*/
	//if(strncmp(inBuff, /*EXAMPLE*/"EX", len) == 0){ return EXAMPLE; }
	/*----------------------------------------------------------------------------------------*/
	if (strncmp(inBuff, /*"ALL"*/"AL", /*3*/len) == 0) {
		return ALL;
	}
	else if (strncmp(inBuff, /*DETAILS*/"DE", len) == 0) {
		return DETAILS;
	}
	else if (strncmp(inBuff, /*PAUSE*/"PA", /*5*/len) == 0) {
		return PAUSE_CONSOLE;
	}
	return 0;
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