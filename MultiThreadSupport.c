#include "MultiThreadSupport.h"

int ThreadCreation(ThreadNamePtr_fn* ThreadName, ThreadsStruct_t *ThreadStruct, int Data_Of_Thread)
{
	int res = 0;
	ThreadStruct->Data_Of_Thread = Data_Of_Thread;

	// variable to hold handle of Thread
	ThreadStruct->Handle_Of_Thread = 0;
	ThreadStruct->ptrToThread = ThreadName;

	ThreadStruct->Handle_Of_Thread = CreateThread(NULL, 0, ThreadStruct->ptrToThread, 
		&ThreadStruct->Data_Of_Thread, 0, NULL);
	if (ThreadStruct->Handle_Of_Thread == NULL) {
		res = -1;
		ExitProcess(ThreadStruct->Data_Of_Thread);
	}
	return res;
}

int ThreadInit(/*ThreadsStruct_t* ThreadStruct*/LPVOID lpParam)
{
	int res = 0;
	int     Data = 0;
	char a;
	HANDLE  hStdout = NULL;
	// Get Handle To screen. Else how will we print?
	if ((hStdout = GetStdHandle(STD_OUTPUT_HANDLE)) == INVALID_HANDLE_VALUE)
		return -1;
	// Cast the parameter to the correct
// data type passed by callee i.e main() in our case.
	Data = *((int*)lpParam);
	return res;
}