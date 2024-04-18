#ifndef _MASTERIMMITATIONCFG_H_
#define _MASTERIMMITATIONCFG_H_
#include <stdint.h>

char mastersMessageId[];

typedef struct{
  uint8_t Status;
  uint32_t lastReadedLine; //from IO File
  uint16_t SlavesAddressToTalk;
  uint8_t  function;
  uint16_t AddressOfSlavesMemoryToTalk;
  uint16_t LenDataToTalk;
  uint16_t communicationPeriod;
  uint8_t* dataToWrite;
}thisMastercfgs_t;

thisMastercfgs_t ThisMastersConfigs;
#endif //_MASTERIMMITATIONCFG_H_