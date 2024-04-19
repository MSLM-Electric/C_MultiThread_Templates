#include <stdint.h>

enum cmdsValEnums {
	ALL = 1,  //it shows all states of your functns
	DETAILS,
	PAUSE_CONSOLE,
	ENABLE_TIMER,
	MAKE_PACKET,
	DMA_ENABLE,
	START_COMMUNICATION,
	STOP_COMMUNICATION,
	EXAMPLE, /*//Users code*/
	cmdsValEnumslastINDEX
};

#define OPTION_NAMES_LEN 20
typedef struct {
	char optionName[OPTION_NAMES_LEN];
	enum cmdsValEnums optionId;
}menuChoise_t;

static menuChoise_t MenuOption[cmdsValEnumslastINDEX-1] = {
	{"ALL", ALL},
	{"DETAILS", DETAILS},
	{"PAUSE", PAUSE_CONSOLE},
	{"TIMENABLE", ENABLE_TIMER},
	{"MAKEPACKET", MAKE_PACKET},
	{"DMAONOFF", DMA_ENABLE},
	{"STARTCOMMUNIC", START_COMMUNICATION},
	{"STOPCOMMUNIC", STOP_COMMUNICATION},
	//Users code
	/*------------------------------Put your Functions launch here----------------------------*/
	{"EXAMPLE", EXAMPLE},
	/*----------------------------------------------------------------------------------------*/
};

struct ConsolesMenuHandle_Type {
	uint8_t CMD[cmdsValEnumslastINDEX - 1];  //mb sounds better: IsEnabledCMD[] or IsCMDEnabled[]
};
struct ConsolesMenuHandle_Type ConsolesMenuHandle;

uint8_t StringCompareAndParseToNum(char* inBuff, uint8_t maxPossibleLen);
void SettingsCMD_Handling(char* inBuff, const uint16_t maxPossibleLen);
int ScanKeyboardWithWhiteSpaces(char* inBuff, uint16_t maxPossibleLen);
void ScanCMDsScenarios(char* buffer, const int maxPossibleLen);