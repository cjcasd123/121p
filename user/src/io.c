#include "Config.h"

// 开漏输出：	PXM1 H   PXM0 H		// 内部上拉断开	－－省电
// 高阻输入：	PXM1 H   PXM0 L		// 电流既不流入，也不流出！－－省电
// 上拉双向：	PXM1 L   PXM0 L		// 弱上拉，灌电流可达20mA，拉电流270uA 
// 推挽输出：	PXM1 L   PXM0 H		// 可达20mA，需加限流电阻；（芯片整体流入/流出电流不超过90mA）


// 电源
//u8 xdata g_bPower;
//u8 xdata iPWR_In;
// LED
u8 xdata g_bLedRun, g_bLedSOS, g_bLedLamp, g_bLedCd;
//u8 xdata g_bLedRunBak, g_bLedQjBak, g_bLedLampBak;
bit g_isLedBak;
bit isFlash, isFlashFast;



/* 根据变量实际执行IO口的灯 */
void pLedRun()    
{

	//运行灯
	switch (g_bLedRun)
	{
	case LED_OFF:
		IO_LED_RUN = OFF_LED;
		break;
	case LED_ON:
		IO_LED_RUN = ON_LED;
		break;
	case LED_FLASH_FAST:
		IO_LED_RUN = isFlashFast;
		break;
	case LED_FLASH:
		IO_LED_RUN = isFlash;
		break;
	default:
		break;
	}

	//求救
	switch (g_bLedSOS)
	{
	case LED_OFF:
		IO_LED_SOS = OFF_LED;
		break;
	case LED_ON:
		IO_LED_SOS = ON_LED;
		break;
	case LED_FLASH_FAST:
		IO_LED_SOS = isFlashFast;
		break;
	case LED_FLASH:
		IO_LED_SOS = isFlash;
		break;
	default:
		break;
	}
#if 0
	//充电灯
	switch (iPWR_In)
	{
	case 0:
		g_bLedCd = LED_OFF;
		break;
	case 1:
		g_bLedCd = LED_FLASH;
		break;
	case 2:
		g_bLedCd = LED_ON;
		break;
	}

	//照明灯
	switch (g_bLedLamp)
	{
	case LED_OFF:
		IO_LED_LAMP1 = OFF_LED;
		IO_LED_LAMP2 = OFF_LED;
		break;
	case LED_ON:
		IO_LED_LAMP1 = ON_LED;
		IO_LED_LAMP2 = ON_LED;
		break;
	case LED_FLASH_FAST:
		IO_LED_LAMP1 = isFlashFast;
		IO_LED_LAMP2 = isFlashFast;
		break;
	case LED_FLASH:
		IO_LED_LAMP1 = isFlash;
		IO_LED_LAMP2 = isFlash;
		break;
	default:
		break;
	}

	//充电指示灯
	switch (g_bLedCd)
	{
	case LED_OFF:
		IO_LED_PWRIN = OFF_LED;
		break;
	case LED_ON:
		IO_LED_PWRIN = ON_LED;
		break;
	case LED_FLASH_FAST:
		IO_LED_PWRIN = isFlashFast;
		break;
	case LED_FLASH:
		IO_LED_PWRIN = isFlash;
		break;
	default:
		break;
	}
#endif
}

void LED_pro(void)
{
	if (g_isS2Sleep == 0)
	{
		/* 开机常亮且还没进行通信  */
		if (g_isCommErr == 1)
			g_bLedRun = 1;   
        // 当为0时进行频闪
	}
	/* 息屏时关灯 */
	else
	{
		g_bLedSOS = 0;
		g_bLedRun = 0;
	}
	pLedRun();
}

#if 0
/* 设置指示灯的参数量 */
void LED_pro(void)  
{
	static u8 bLastTime;
	static bit isUpFlash;


	// 这个是暂时的！！！！！
	u8 g_bPowerUpTimeOut1s, g_bPowerDownTimeOut1s;
	//开机指示灯设置
	if (g_bPowerUpTimeOut1s > 0)
	{
		if (bLastTime != g_bPowerUpTimeOut1s)
		{
			isUpFlash = !isUpFlash;

			if (isUpFlash)
			{
				g_bLedRun = LED_ON;
				g_bLedSOS = LED_OFF;
				g_bLedLamp = LED_OFF;
			}
			else
			{
				g_bLedRun = LED_OFF;
				g_bLedSOS = LED_ON;
				g_bLedLamp = LED_OFF;
			}

			bLastTime = g_bPowerUpTimeOut1s;
		}
	}

	//关机指示灯设置（准备关闭）
	else if (g_bPowerDownTimeOut1s > 0)
	{
		g_bLedRun = LED_ON;
		g_bLedSOS = LED_ON;
		g_bLedLamp = LED_OFF;
	}

	//关机则全部指示灯灭（真的关闭）
	else if (g_isS2Sleep)
	{
		g_bLedRun = LED_OFF;
		g_bLedSOS = LED_OFF;
		g_bLedLamp = LED_OFF;
	}
	else       // 设备运行时的提示灯
	{
		//通讯故障（保存数据并全灯灭）
		if (g_isCommErr)
		{
			//通讯故障需要备份LED状态
			if (!g_isLedBak)
			{
				g_isLedBak = 1;
				g_bLedRunBak = g_bLedRun;
				g_bLedLampBak = g_bLedLamp;
				g_bLedQjBak = g_bLedSOS;
			}
			g_bLedRun = LED_OFF;
			g_bLedLamp = LED_OFF;
			g_bLedSOS = LED_OFF;
			//IO_PA_SHAN = 0;
		}
		// 正常通讯（全灯亮）
		else
		{
			if (g_isLedBak)
			{
				g_isLedBak = 0;
				g_bLedRun = g_bLedRunBak;
				g_bLedLamp = g_bLedLampBak;
				g_bLedSOS = g_bLedQjBak;
			}
		}
	}

	pLedRun();

}
#endif
/* IO口初始化 */
void IO_Init(void)  // 一般默认是准双向的故现在不进行修改   ---- 而且特别注意usb_boot管脚（这个是一定为开漏拉低！！！！！）
{
	//未使用管脚，－－准双向，省电
	P0M0 = P0M1 = 0;
	P1M0 = P1M1 = 0;
	P2M0 = P2M1 = 0;
	P3M0 = P3M1 = 0;
	P4M0 = P4M1 = 0;

	//LED控制管脚初始化--准双向
	P3M1 &= ~(1 << 4);			// 运行
	P3M0 &= ~(1 << 4);
	P3M1 &= ~(1 << 5);			// 求救
	P3M0 &= ~(1 << 5);

//	P2M1 &= ~(1 << 1);			// 充电
//	P2M0 &= ~(1 << 1);

	// 手电筒：推挽和双向无差别；
//	P2M1 &= ~((1 << 2) | (1 << 3));
//	P2M0 &= ~((1 << 2) | (1 << 3));

	//KEY管脚初始化P3.2/3--准双向 
	P3M1 &= ~((1 << 2) | (1 << 3));
	P3M0 &= ~((1 << 2) | (1 << 3));
//	P1M1 &= ~((1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6));
//	P1M0 &= ~((1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6));

#if 0
	// 充电检测--高阻 
	P2M1 |= (1 << 4);
	P2M0 &= ~(1 << 4);
	IO_DECT_AC = 0;
	// 充满检测--高阻
	P3M1 |= (1 << 7);
	P3M0 &= ~(1 << 7);
	IO_DECT_CHG = 0;
	// 功放使能--推挽
	P0M1 &= ~(1 << 2);
	P0M0 |= (1 << 2);
	IO_PA_SHAN = 0;			// 低禁止，脉冲使能
#endif

}


/* IO省电处理 */
void GPIO_SavePower(void)
{
	//GSM_READY = 0;						// S2实测无效管脚！！！串接电阻断开！
	//GSM_PWRKEY = 1;						// 拉低开机	
	//GSM_RESET = 1;						// 低电平 > 31.25ms 复位


	// 充电检测--高阻 
	//IO_DECT_AC = 0;
	// 充满检测--高阻
	//IO_DECT_CHG = 0;
	//IO_PA_SHAN = 0;			// 低禁止，脉冲使能

	g_bLedRun = LED_OFF;
	g_bLedCd = LED_OFF;
	g_bLedLamp = LED_OFF;
	g_bLedSOS = LED_OFF;
	IO_LED_RUN = OFF_LED;		// 运行指示
	IO_LED_SOS = OFF_LED;		// 求救指示
#if 0
	IO_LED_PWRIN = OFF_LED;		// 充电指示
	IO_LED_LAMP1 = OFF_LED;		// 手电筒
	IO_LED_LAMP2 = OFF_LED;
#endif
}

#if 0
/* 充电检测 */
void Check_AC_In(void)
{
	// 充电断开检测、充满检测
	if (bAC_In) {
		IO_DECT_AC = 1;
		if (!IO_DECT_AC) {		// 高电平充电
			iDect_AC_Count++;
			if (iDect_AC_Count >= 3) {
				iDect_AC_Count = 0;
				bAC_In = 0;

				iPWR_In = 0;
				PrintString1("CHG OUT");
				g_bPower = '0';
			}
		}

		// 充满检测
		if (bDect_Chg) {
			IO_DECT_CHG = 0;
			if (!IO_DECT_CHG) {
				iDect_Bat_Count++;
				if (iDect_Bat_Count >= 3)
				{
					iDect_Bat_Count = 0;
					bDect_Chg = 0;
					if (iPWR_In == 2)
					{
						iPWR_In = 1;		//转为继续充电状态
					}

				}

			}

		}
		else {
			IO_DECT_CHG = 1;
			if (IO_DECT_CHG) {
				iDect_Bat_Count++;
				if (iDect_Bat_Count >= 3)
				{
					iDect_Bat_Count = 0;
					bDect_Chg = 1;
					iPWR_In = 2;
					PrintString1("BAT Full");
					g_bPower = '2';
				}

			}
		}
		// 充电检测	
	}
	else {
		IO_DECT_AC = 0;
		if (IO_DECT_AC) {		// 高电平充电
			iDect_AC_Count++;
			if (iDect_AC_Count >= 3) {
				iDect_AC_Count = 0;
				bAC_In = 1;
				iPWR_In = 1;
				PrintString1("CHG INSER");
				g_bPower = '1';
			}
		}
	}
}
#endif



