#ifndef _TYPE_H_
#define _TYPE_H_
#include <LIMITS.h>

typedef signed char        int8_t;
typedef int                int16_t;
typedef long			   int32_t;
typedef unsigned char      uint8_t;
typedef unsigned int       uint16_t;
typedef unsigned long	   uint32_t;


typedef signed char        s8;
typedef int                s16;
typedef long			   s32;
typedef unsigned char      u8;
typedef unsigned int       u16;
typedef unsigned long	   u32;

#define INT8_MIN          (-127-1)
#define INT16_MIN         (-32767-1)
#define INT32_MIN         (-2147483647-1)
#define INT8_MAX          127
#define INT16_MAX         32767
#define INT32_MAX         2147483647
#define UINT8_MAX         0xffu
#define UINT16_MAX        0xffffu
#define UINT32_MAX        0xffffffffu


#define INT8_C(x)    (x)
#define INT16_C(x)   (x)
#define INT32_C(x)   (x ## L)

#define UINT8_C(x)   (x)
#define UINT16_C(x)  (x)
#define UINT32_C(x)  (x ## UL)

#define INTMAX_C(x)  INT64_C(x)
#define UINTMAX_C(x) UINT64_C(x)




#define TRUE		1
#define FALSE		0
#define	TimeOutSet1		5
//#define	TimeOutSet2		300
#define	TimeOutSet2		200
//#define COM1_RX_Lenth	64
//#define COM1_TX_Lenth	64	
//#define COM2_RX_Lenth	64
//#define COM2_TX_Lenth	64

#define my_time()			g_time_ms     // 设置时间轴的定义

/* 
   最后需要开启的寄存器  开启串口和总中断  
   其他有用到的再进行加入
*/

#define enableExtiInterrupts()      EA = 1
#define disableExtiInterrupts()     EA = 0

#define enableUart1Interrupts()		ES = 1
#define disableUart1Interrupts()	ES = 0

#define enableUart2Interrupts()		IE2 = 1
#define disableUart2Interrupts()	IE2 = 0

#define enableUart3Interrupts()		IE2 |= 0x08;
#define disableUart3Interrupts()	IE2 = 0

#define enableUart4Interrupts()		IE2 |= 0x10;
#define disableUart4Interrupts()	IE2 = 0

#endif 
