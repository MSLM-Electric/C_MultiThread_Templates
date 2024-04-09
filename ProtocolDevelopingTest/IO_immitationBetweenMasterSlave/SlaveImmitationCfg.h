#ifndef _SLAVEIMMITATIONCFG_H_
#define _SLAVEIMMITATIONCFG_H_
#include <stdint.h>

typedef struct{
  uint8_t Status;
  uint32_t lastReadedLine; //from IO File
  uint16_t MyAddress;
  uint16_t AddressOfMineMemoryToBeginTransmit;
  uint16_t LenDataToWrite;
}thisSlavecfgs_t;
#endif //_SLAVEIMMITATIONCFG_H_