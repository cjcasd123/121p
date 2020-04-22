#include "Config.h"

/*
       内部资源说明
  1.使用内部时钟  ---- 12MHZ
  2.使用串口1--打印   使用串口2--通讯   波特率9600
  3.使用定时器0 --- 1MS中断

  修改了接收返回值的字节置1或者0

  4.通信协议目前主要时接收和发送是一体的
*/

sbit usb_boot = P1 ^ 7;
sbit mcu_pkey = P1 ^ 6;
sbit c_lte = P3 ^ 5;

/*全局变量定义*/
volatile uint8_t iEvent_Flag = 0;					/* 系统标志位 */
//volatile uint8 wWdog_1S_Timer = 0;
uint8_t iWdog_S = 0;
bit bAC_In = 0, bDect_Chg = 0;
u8  iDect_AC_Count = 0;
u8  iDect_Bat_Count = 0;

/*静态变量定义*/
static uint8_t iTimer_Counter1 = 0;	  			// 100ms计时
static uint8_t iTimer_Counter2 = 0;				// 1s计时

u8  g_bPlusTimeOut1s;
u8  g_bSleepTimeOut1s;
u8  bFreeTimeOut1s;
bit g_isS2Sleep;
bit g_isS2On, g_isS2Off, g_isS2Reset;



//开机参数
void ResetPara(void)
{
	g_isCommErr = 1;
	g_isS2Sleep = 0;
	bFreeTimeOut1s = 5;
	g_bPlusTimeOut1s = 2;
	g_bKey[0] = 0;
	g_bKey[1] = 0;
}



/*主函数*/
void main(void)
{

	//  将下载IO进行拉低
	usb_boot = 0;
	// MCU初始化						
	disableExtiInterrupts();
	p_sysclk_init();
	p_timer_init();
	//p_uart1_init();
	p_uart2_init();
	//IO_Init();   --- 都是准双向故暂时不用
	//INT0_Init();
	enableExtiInterrupts();
	/* 开启模块并下载 */
	mcu_pkey = 0;
	c_lte = 0;
	Delay_100ms(40);
	// 下载口设置为开漏输出
//	PORT_MODE_SET(1, 7, PORT_MODE_HZ);
//#if 0
	P1M1 |= (1 << 7);
	P1M0 |= (1 << 7);
	usb_boot = 0;
	Delay_100ms(40);
//#endif
	/**************************************************/
	//g_bPower = 0;
	g_isS2On = 1;
	// 开机参数配置
	ResetPara();

	while (1)
	{

		//按键5秒抛掉
		if ((bKeyTimeOut1s == 0) && ((g_bKey[0] != 0) || (g_bKey[1] != 0)))
		{
			g_bKey[0] = 0;
			g_bKey[1] = 0;
		}
#if 0
		// 关机  进入休眠事件（按键唤醒模式和定时唤醒）
		// 直接进行关机   --- 一开始不进入关机
		if (g_isS2Sleep)
		{
			// 常按6按键
			if ((g_bKey[0] == 0x86) || (g_bKey[1] == 0x86))
			{
				g_isS2On = 1;
				g_bKey[0] = 0;
				g_bKey[1] = 0;

				g_isS2Sleep = 0;
			}
			// IO口反初始化
			GPIO_SavePower();

			//手动关机：只检测电源键
			//正常休眠（无按键5秒）：定时唤醒（如何接收指示灯？ 如何控制指示灯？）
			while (g_bSleepTimeOut1s == 0)
			{
				EX0 = 1;				// 打开外部中断
				PCON = 0x02;            // 进入休眠模式 （VOCTRL = 0x00;）这个则是默认故可以不写 
				_nop_();
				_nop_();
				_nop_();
				_nop_();
				EX0 = 0;				//唤醒则关闭外部中断
			}
			// IO口初始化
		}
#endif

#if 0
		if (sign_date == 5)
		{
			//P1M1 |= (1 << 7);
			//P1M0 |= (1 << 7);
			//usb_boot = 0;
			P1M1 |= (1 << 7);
			P1M0 &= ~(1 << 7);
			IO_LED_RUN = ~IO_LED_RUN;
			IO_LED_SOS = ~IO_LED_SOS;
			sign_date = 0;
		}
#endif


		// 关机和运行 --- 没有执行任务则是睡眠
		//else
		//if (g_isS2Sleep == 1)   // 这里修改了
		if (g_isS2Sleep == 0)
		{
			if ((g_bKey[0] == 0x86) || (g_bKey[1] == 0x86))
			{
				g_isS2Off = 1;
				g_bKey[0] = 0;
				g_bKey[1] = 0;
			}
  
			//MCU_IDLE();
			//没有任务则休眠
			//休眠：100ms定时唤醒执行LED
			if (pSleepAble())
			{
				static u8 iSleep_Cnt;

				WakeTimerSet(160);			// 100ms（掉电定时器）
				PCON = 0x02;
				_nop_();
				_nop_();
				_nop_();
				_nop_();

				isFlashFast = !isFlashFast;		//计时
				iSleep_Cnt++;
				if (iSleep_Cnt >= 10)
				{
					iSleep_Cnt = 0;
					isFlash = !isFlash;

					if (g_bPlusTimeOut1s > 0)
					{
						g_bPlusTimeOut1s--;
					}
				}

				//	WDT_CONTR |= WDT_CLR;
				//	iEvent_Flag |= EVENT_TIMER_100MS;
			}
  


#if 0
			//20毫秒定时处理	
			if (iEvent_Flag & EVENT_TIMER_100MS)
			{
				iEvent_Flag &= (uint8)(~EVENT_TIMER_100MS);
				// 设备充电检测
				//Check_AC_In();
				// 指示灯参数的配置
				if (bFreeTimeOut1s == 0)
				{
					//LED_pro();
				}

				//  通讯是否正常 
				if (g_isCommErr)
				{
					if (g_bCommErrTimeOut01s < 0xff)
					{
						g_bCommErrTimeOut01s++;
					}
					else
					{

						if (g_isS2CommSleep == 0)
						{
							g_isS2CommSleep = 1;
							PrintString1("ComErr TimeOut\r\n");
						}

						//	PrintString1("ComErr TimeOut\r\n");
					}
				}
			}
#endif
			/* 接收发送 */
			pUart_Pro();
		}
	}
}




/* 定时器0 时间戳  --- 1MS */
void TM0_Isr() interrupt 1 using 3
{

	static u8 bCnt;
	bCnt++;
	++g_time_ms;
	if (COM2.RX_TimeOut > 0)
	{
		COM2.RX_TimeOut--;
	}
//	if (COM1.RX_TimeOut > 0)
//	{
//		COM1.RX_TimeOut--;
//	}

	if (bCnt >= 20)  //  --- 20ms
	{
		bCnt = 0;
		/* 扫描哪个按键按下和时间的长度 */
		pAddKey(0, KEY1 == 0);
		pAddKey(1, KEY2 == 0);
		if (COM2.TX_TimeOut > 0)
		{
			COM2.TX_TimeOut--;
		}

		// 100毫秒计时
		iTimer_Counter1++;
		if (iTimer_Counter1 >= 5)   // 100毫秒  1*20*5
		{
			iTimer_Counter1 = 0;

			iEvent_Flag |= EVENT_TIMER_100MS;

			isFlashFast = !isFlashFast;			//快闪

			// 1秒计时   100*10
			iTimer_Counter2++;
			if (iTimer_Counter2 >= 10)
			{
				iTimer_Counter2 = 0;

				isFlash = !isFlash;					//慢闪	
				/**********************************************/
				// 查询是否有按键按下
				if (bFreeTimeOut1s > 0)
				{
					bFreeTimeOut1s--;
				}
				// 心跳包的延时
				if (g_bPlusTimeOut1s > 0)
				{
					g_bPlusTimeOut1s--;
				}
				// 暂时不用
				if (g_bSleepTimeOut1s > 0)
				{
					bFreeTimeOut1s = 5;
					g_bSleepTimeOut1s--;
				}
#if 0
				if (g_bPowerUpTimeOut1s > 0)
				{
					bFreeTimeOut1s = 5;
					g_bPowerUpTimeOut1s--;
				}

				if (g_bPowerDownTimeOut1s > 0)
				{
					bFreeTimeOut1s = 5;
					g_bPowerDownTimeOut1s--;
				}
#endif
				// 测试按键长按和短按
				if (bKeyTimeOut1s > 0)
				{
					bKeyTimeOut1s--;
				}
				/**************************************************/

			}
		}
		//LED_pro();    // 20MS查询一次灯
	}

}








