#include "ConsoleMenuOptions.h"

uint8_t StringCompareAndParseToNum(char* inBuff, uint8_t maxPossibleLen)
{
	if (inBuff == NULL)
		return -1;
	uint8_t len = maxPossibleLen;
	if (maxPossibleLen == NULL)
		len = 2;

	//Users code
	/*---------Put your Functions launch to the header file ConsoleMenuOptions.h----------*/
	/*-------------------------------------------------------------------------------------*/
	for (uint8_t u = 0; u < sizeof(MenuOption); u++) {
		if (strncmp(inBuff, MenuOption[u].optionName, len) == 0)
			return MenuOption[u].optionId;
	}
	return 0;
}