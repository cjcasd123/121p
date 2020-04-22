#ifndef _IO_H_
#define _IO_H_
#include "Config.h"

//LED
sbit IO_LED_RUN = P3 ^ 4;		// 运行指示
sbit IO_LED_SOS = P5 ^ 5;		// 求救指示

#if 0
/*硬件接口定义*/
sbit IO_DECT_AC = P3 ^ 7;		// 充电检测
sbit IO_DECT_CHG = P2 ^ 4;		// 充满检测
sbit IO_PA_SHAN = P0 ^ 2;		// 运放使能
sbit IO_RXD2 = P1 ^ 0;
//LED
sbit IO_LED_RUN = P3 ^ 4;		// 运行指示
sbit IO_LED_QJ = P3 ^ 5;		// 求救指示
sbit IO_LED_PWRIN = P2 ^ 1;	// 充电指示
sbit IO_LED_LAMP1 = P2 ^ 2;	// 手电筒
sbit IO_LED_LAMP2 = P2 ^ 3;
#endif
//电源
//extern u8 xdata g_bPower;
//LED
extern u8 xdata g_bLedRun, g_bLedSOS, g_bLedLamp, g_bLedCd;
extern bit isFlash, isFlashFast;
extern bit g_isLedBak;
//---------------------------------------------------------------------
//LED
enum
{
	LED_OFF = 0,
	LED_ON,
	LED_FLASH_FAST,
	LED_FLASH,
};
enum
{
	ON_LED = 0,
	OFF_LED,
};

/*外部函数声明*/
void IO_Init(void);								
void Check_AC_In(void);						
void LED_pro(void);
void GPIO_SavePower(void);
#endif

