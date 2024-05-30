#ifndef CONSOLEMENUOPTIONS_H
#define CONSOLEMENUOPTIONS_H

#include <stdint.h>
#include "Lib/type_def.h"

enum cmdsValEnums {
	cmdsValEnumsfirstINDEX = 1,
	ALL = cmdsValEnumsfirstINDEX,  //it shows all states of your functns
	DETAILS,
	PAUSE_CONSOLE,
	ENABLE_TIMER,
	MAKE_PACKET,
	SLAVE_CFG,
	DMA_ENABLE,
	START_COMMUNICATION,
	STOP_COMMUNICATION,
	SET_TIMER_PERIOD,
	TRACE_PORT,
	TRACE_CONFIGS,
	READ_BUS,
	COMMON_CONFIGS,
	EXAMPLE, /*//Users code*/
	cmdsValEnumslastINDEX
};

#define OPTION_NAMES_LEN 20
typedef struct {
	char optionName[OPTION_NAMES_LEN];
	enum cmdsValEnums optionId;
}menuChoise_t;

static menuChoise_t MenuOption[cmdsValEnumslastINDEX - cmdsValEnumsfirstINDEX] = {
	{"all", ALL},
	{"details", DETAILS},
	{"pause", PAUSE_CONSOLE},
	{"timenable", ENABLE_TIMER},
	{"makepack", MAKE_PACKET}, //"MAKEPACKET"
	{"slavecfg", SLAVE_CFG },
	{"dmaonoff", DMA_ENABLE},
	{"startcom", START_COMMUNICATION}, //"STARTCOMMUNIC"
	{"stopcom", STOP_COMMUNICATION},   //"STOPCOMMUNIC"
	{"setperio", SET_TIMER_PERIOD},
	{"trace", TRACE_PORT},
	{"tracecfg", TRACE_CONFIGS},
	{"readbus", READ_BUS},
	{"commoncfg", COMMON_CONFIGS},
	//Users code
	/*------------------------------Put your Functions launch here----------------------------*/
	{"example", EXAMPLE},
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