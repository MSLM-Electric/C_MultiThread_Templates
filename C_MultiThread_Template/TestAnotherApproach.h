#include <stdint.h>

enum cmdsValEnums {
	ALL = 1,  //it shows all states of your functns
	DETAILS,
	PAUSE_CONSOLE,
	ENABLE_TIMER,
	EXAMPLE, /*//Users code*/
};

typedef struct {
	char optionName[3];
	enum cmdsValEnums optionId;
}menuChoise_t;

menuChoise_t MenuOption[5] = {
	{"AL", ALL},
	{"DE", DETAILS},
	{"PA", PAUSE_CONSOLE},
	{"TI", ENABLE_TIMER},
	//Users code
	/*------------------------------Put your Functions launch here----------------------------*/
	{"EX", EXAMPLE},
	/*----------------------------------------------------------------------------------------*/
};