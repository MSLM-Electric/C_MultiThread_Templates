#ifndef _MASTERIMMITATIONCFG_H_
#define _MASTERIMMITATIONCFG_H_
#include <stdint.h>

const char mastersMessageId[] = "MASTER_WRITE:";
const char slavesMessageId[] =  "SLAVE__WRITE:";

typedef struct{
  //status;
  uint32_t lastReadedLine; //from IO File
}thisMastercfgs_t;
#endif //_MASTERIMMITATIONCFG_H_