#include <stdint.h>
#include "Lib/SimpleTimerWP.h" //!+ if remove it |NOTE.1|

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
	EXAMPLE, /*//Users code*/
	cmdsValEnumslastINDEX
};

#define OPTION_NAMES_LEN 20
typedef struct {
	char optionName[OPTION_NAMES_LEN];
	enum cmdsValEnums optionId;
}menuChoise_t;

static menuChoise_t MenuOption[cmdsValEnumslastINDEX-cmdsValEnumsfirstINDEX] = {
	{"ALL", ALL},
	{"DETAILS", DETAILS},
	{"PAUSE", PAUSE_CONSOLE},
	{"TIMENABLE", ENABLE_TIMER},
	{"MAKEPACK", MAKE_PACKET}, //"MAKEPACKET"
	{"DMAONOFF", DMA_ENABLE},
	{"STARTCOM", START_COMMUNICATION}, //"STARTCOMMUNIC"
	{"STOPCOM", STOP_COMMUNICATION},   //"STOPCOMMUNIC"
	{"SETPERIO", SET_TIMER_PERIOD},
	//Users code
	/*------------------------------Put your Functions launch here----------------------------*/
	{"EXAMPLE", EXAMPLE},
	/*----------------------------------------------------------------------------------------*/
};

struct ConsolesMenuHandle_Type {
	uint8_t CMD[cmdsValEnumslastINDEX - cmdsValEnumsfirstINDEX];  //?!mb sounds better: IsEnabledCMD[] or IsCMDEnabled[]
	//void *callback_fn[cmdsValEnumslastINDEX - cmdsValEnumsfirstINDEX](void *);
};
struct ConsolesMenuHandle_Type ConsolesMenuHandle;
extern Timerwp_t MonitoringTim; //!+ and if remove it |NOTE.2| we get the separate settings tester by console for any independent project.

uint8_t StringCompareAndParseToNum(char* inBuff, uint8_t maxPossibleLen);
void SettingsCMD_Handling(char* inBuff, const uint16_t maxPossibleLen);
int ScanKeyboardWithWhiteSpaces(char* inBuff, uint16_t maxPossibleLen);
void ScanCMDsScenarios(char* buffer, const int maxPossibleLen);