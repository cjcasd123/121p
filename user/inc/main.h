
#ifndef _MAIN_H_
#define _MAIN_H_


/*iEvent_Flag事件标志定义*/
#define	EVENT_TIMER_100MS				0x02 		// 20毫秒定时标识

// 看门狗互锁定义
#define WDOG_1 						1
#define WDOG_2 						2

#define WDT_CLR	  					0x10		// 看门狗计时清零

#define		WakeTimerSet(scale)		WKTCL = (scale) % 256,WKTCH = (scale) / 256 | 0x80	/* WKTEN = 1	允许睡眠唤醒定时器 */

/*全局变量声明*/
extern volatile uint8_t iEvent_Flag;
extern bit g_isS2Sleep;
extern bit g_isS2On, g_isS2Off, g_isS2Reset;
extern volatile uint8_t iWdog_1S_Timer;
extern uint8_t iWdog_S;
extern bit bAC_In, bDect_Chg;
extern u8  iDect_AC_Count;
extern u8  iDect_Bat_Count;

extern u8  g_bPlusTimeOut1s;
extern u8  bFreeTimeOut1s;




#endif


/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/