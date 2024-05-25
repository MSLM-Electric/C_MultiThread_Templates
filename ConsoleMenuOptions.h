#ifndef CONSOLEMENUOPTIONS_H
#define CONSOLEMENUOPTIONS_H

#include <stdint.h>

enum cmdsValEnums {
	cmdsValEnumsfirstINDEX = 1,
	ALL = cmdsValEnumsfirstINDEX,  //it shows all states of your functns
	DETAILS,
	PAUSE_CONSOLE,
	ENABLE_TIMER,
	MAKE_PACKET,
	DMA_ENABLE,
	START_COMMUNICATION,
	STOP_COMMUNICATION,
	SET_TIMER_PERIOD,
	TRACE_PORT,
	EXAMPLE, /*//Users code*/
	cmdsValEnumslastINDEX
};

#define OPTION_NAMES_LEN 20
typedef struct {
	char optionName[OPTION_NAMES_LEN];
	enum cmdsValEnums optionId;
}menuChoise_t;

static menuChoise_t MenuOption[cmdsValEnumslastINDEX - cmdsValEnumsfirstINDEX] = {
	{"ALL", ALL},
	{"DETAILS", DETAILS},
	{"PAUSE", PAUSE_CONSOLE},
	{"TIMENABLE", ENABLE_TIMER},
	{"MAKEPACK", MAKE_PACKET}, //"MAKEPACKET"
	{"DMAONOFF", DMA_ENABLE},
	{"STARTCOM", START_COMMUNICATION}, //"STARTCOMMUNIC"
	{"STOPCOM", STOP_COMMUNICATION},   //"STOPCOMMUNIC"
	{"SETPERIO", SET_TIMER_PERIOD},
	{"TRACE", TRACE_PORT},
	//Users code
	/*------------------------------Put your Functions launch here----------------------------*/
	{"EXAMPLE", EXAMPLE},
	/*----------------------------------------------------------------------------------------*/
};

typedef void* (callback_fn)(char* buffer, const int maxPossibleLen, void* arg);

struct ConsolesMenuHandle_Type {
	uint8_t CMD[cmdsValEnumslastINDEX - cmdsValEnumsfirstINDEX];  //?!mb sounds better: IsEnabledCMD[] or IsCMDEnabled[]
	callback_fn* executeFunc[cmdsValEnumslastINDEX - cmdsValEnumsfirstINDEX];
};
struct ConsolesMenuHandle_Type ConsolesMenuHandle;

uint8_t StringCompareAndParseToNum(char* inBuff, uint8_t maxPossibleLen);
void SettingsCMD_Handling(char* inBuff, const uint16_t maxPossibleLen);
int ScanKeyboardWithWhiteSpaces(char* inBuff, uint16_t maxPossibleLen);
void ScanCMDsScenarios(char* buffer, const int maxPossibleLen);
#define PORTABILITY_WITH_OLD_NAME //If you see this attached to some function, so it's for a while/temporarely. After renaming and fixing all corresponded usings the new name of func will be taken. 
#define ExecuteConsoleMenuRegisteredCallbacksFunc ScanCMDsScenarios PORTABILITY_WITH_OLD_NAME  //ExecuteRegisteredCallbacksFuncOfCMDs
#endif // !CONSOLEMENUOPTIONS_H