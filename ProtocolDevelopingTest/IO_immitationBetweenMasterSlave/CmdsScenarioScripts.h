#ifndef CMDSSCENARIOSCRIPT_H
#define CMDSSCENARIOSCRIPT_H

#include <stdint.h>
#include <stdio.h>
#ifdef MASTER_PORT_PROJECT
#include "MasterImmitationCfg.h"
#else //if SLAVE_PORT_PROJECT
#include "SlaveImmitationCfg.h"
#endif // MASTER_PORT_PROJECT
//#include "PortsBusMessages.h"
#include "../../ConsoleMenuOptions.h"
#include "../../Lib/type_def.h"

#include "../HardwarePeripheral/HardwareInterfaceUnit.h"

extern Timerwp_t MonitoringTim;

void MakingPacketScenarios(char* buffer, const int maxPossibleLen, void* arg);
void SetTimerPeriodCmdFunction(char* buffer, const int maxPossibleLen, void* arg);
#endif // !CMDSSCENARIOSCRIPT_H