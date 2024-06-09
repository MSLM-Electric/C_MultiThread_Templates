#ifndef TYPE_DEF_H
#define TYPE_DEF_H
#include "stdint.h"
#define     __O     volatile                  /*!< defines 'write only' permissions     */
#define     __IO    volatile                  /*!< defines 'read / write' permissions   */

#if defined __MINGW32__
#define MCU_PACK __attribute__((packed, gcc_struct))
#elif defined DEBUG_ON_VS
#define STRINGIFY(a) #a
#define PRAGMA _Pragma
#define MCU_PACK PRAGMA(STRINGIFY(pack(push, 4)))
#define END_MCU_PACK PRAGMA(STRINGIFY(pack(pop)))
#else
#define MCU_PACK __attribute__((packed))
#endif
typedef signed long long int s64;
typedef signed long int s32;
typedef signed short s16;
typedef signed char s8;
typedef signed char i8;

typedef volatile signed long long int vs64;
typedef volatile signed long int vs32;
typedef volatile signed short vs16;
typedef volatile signed char vs8;
typedef volatile signed char vi8;

typedef uint64_t u64;
typedef unsigned long int u32;
typedef unsigned int ui32;//for get rid of warning messege
typedef unsigned short u16;
typedef unsigned char u8;

typedef const unsigned long long int uc64;
typedef const unsigned long int uc32;
typedef const unsigned short uc16;
typedef const unsigned char uc8;

typedef volatile unsigned long int vu32;
typedef volatile unsigned short vu16;
typedef volatile unsigned char vu8;

typedef volatile unsigned long int const vuc32;
typedef volatile unsigned short const vuc16;
typedef volatile unsigned char const vuc8;

typedef u8 u8_t;
typedef u16 u16_t;
typedef u32 u32_t;

#ifndef TRUE
  #define TRUE 1
#endif
#ifndef FALSE
  #define FALSE 0
#endif

#ifndef NOT
#define NOT !
#endif // !NOT

#ifndef ONLY
#define ONLY  //!just for beautyfying and readabiling/readability code
#endif // !ONLY

#ifndef STILL
#define STILL  //!just for beautyfying and readabiling code
#endif // !STILL

#ifndef NOTHING
#define NOTHING 0 //!just for beautyfying and readability code
#endif // !NOTHING

#ifndef SOMETHING 
#ifdef NULL
#define SOMETHING ~NULL
#endif // NULL
#endif // !SOMETHING 


#ifndef UNUSED
#define UNUSED(X) (void)X      /* To avoid gcc/g++ warnings */
#endif // !UNUSED

#define clearBITS(x) &= ~(x)
#define setBITS(x) |= (x)
//#define BITPOS(x) y = while((x >> 1) > 1){y++};
#define asm __asm
//u16 BitPos(u16 Bit);
//u16 BitPos(u16 Bit)
//{
//	u16 res = 0;
//	while ((Bit >> res) > 1) {
//		res++;
//	}
//	return res;
//}

#endif// TYPE_DEF_H
