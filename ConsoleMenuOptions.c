#include "ConsoleMenuOptions.h"
#ifdef PROTOCOL_DEVELOPING_PROJECT
#include "ProtocolDevelopingTest/HardwarePeripheral/HardwareInterfaceUnit.h"
#endif

static void ShowAllStates(void);

uint8_t StringCompareAndParseToNum(char* inBuff, uint8_t maxPossibleLen)
{
	if (inBuff == NULL)
		return -1;
	uint8_t len = maxPossibleLen;
	if (maxPossibleLen == NULL)
		len = OPTION_NAMES_LEN;

	//Users code
	/*---------Put your Functions launch to the header file ConsoleMenuOptions.h----------*/
	/*-------------------------------------------------------------------------------------*/
	for (uint8_t u = 0; u < sizeof(MenuOption); u++) {
		if (strncmp(inBuff, MenuOption[u].optionName, len) == 0)
			return MenuOption[u].optionId;
	}
	return 0;
}

void SettingsCMD_Handling(char* inBuff, const uint16_t maxPossibleLen)
{
	uint8_t parsedCMD = StringCompareAndParseToNum(inBuff, NULL);
	ConsolesMenuHandle.CMD[parsedCMD] = ~ConsolesMenuHandle.CMD[parsedCMD] & 0x01;
	uint8_t IsParsedCMD_Enabled = ConsolesMenuHandle.CMD[parsedCMD];
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
			ConsolesMenuHandle.CMD[STOP_COMMUNICATION] = 0;
		}
	}break;
	case STOP_COMMUNICATION: {
		if (IsParsedCMD_Enabled) {
			printf("STOP COMMUNICATION\n");
			ConsolesMenuHandle.CMD[START_COMMUNICATION] = 0;
		}
	}break;
	default:
		break;
	}
	memset(inBuff, 0, 2/*maxPossibleLen*/); //memsetstr
	return;
}

int ScanKeyboardWithWhiteSpaces(char* inBuff, uint16_t maxPossibleLen)
{
	int res = -1;
	uint16_t ptr = 0;
	char catchedChar = 0;
	memset(inBuff, 0, maxPossibleLen);
	uint8_t timeout = 200;
	do {
		scanf_s("%s%c", (char*)&inBuff[ptr], maxPossibleLen, &catchedChar, 1);
		ptr = strlen(inBuff);
		if (ptr > maxPossibleLen - 1)
			return -2;
		inBuff[ptr] = ' ';
		ptr++;
	} while ((catchedChar != '\n') && (timeout--)); //(catchedChar == ' ')
	if (timeout == 0)
		return res;
	inBuff[ptr] = '\0';
	return res = 0;
}

void ScanCMDsScenarios(char *buffer, const int maxPossibleLen)
{
	u8 u = 0;
	void* arg = NULL; //UNUSED
	for (u = cmdsValEnumsfirstINDEX; u < cmdsValEnumslastINDEX; u++) {
		if ((ConsolesMenuHandle.executeFunc[u] != NULL) && (ConsolesMenuHandle.CMD[u])) {
			ConsolesMenuHandle.executeFunc[u](buffer, maxPossibleLen, arg);
		}
	}
	return;
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