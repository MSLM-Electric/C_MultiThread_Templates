#include "ConsoleMenuOptions.h"
#include "ProtocolDevelopingTest/HardwarePeripheral/HardwareInterfaceUnit.h"

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
	//char keyboardBuffs[255]; //bad
	//char *keyboardBuffs = (char *)malloc(255 * sizeof(char));
	if (ConsolesMenuHandle.CMD[MAKE_PACKET]) {
		memset(buffer, 0, maxPossibleLen);
		//mutxMaster//!
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
		ConsolesMenuHandle.CMD[MAKE_PACKET] = 0;
	}
	else if (ConsolesMenuHandle.CMD[SET_TIMER_PERIOD]) {
		memset(buffer, 0, maxPossibleLen);
		printf_s("Set Monitoring Timers period:\n");
		scanf_s("%d", buffer);
		MonitoringTim.setVal = *(uint16_t*)&(buffer[0]);
		ConsolesMenuHandle.CMD[SET_TIMER_PERIOD] = 0;
	}
	//free(keyboardBuffs);
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