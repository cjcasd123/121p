#include "usart.h"

bit g_isCommWait, g_isCommErr, g_isCommTry;
u8 g_bCommErrTimeOut01s;
u8 g_bSendNum;

#if 0
COMx_Define	 xdata COM1;
u8	 xdata TX1_Buffer[COM_TX1_Lenth];	//发送缓冲
u8 	 xdata RX1_Buffer[COM_RX1_Lenth];	//接收缓冲
#endif 
COMx_Define	 xdata COM2;
u8	xdata TX2_Buffer[COM_TX2_Lenth];	//发送缓冲
u8 	xdata RX2_Buffer[COM_RX2_Lenth];	//接收缓冲

static u8 xdata bTxLen;
static u8 xdata bTxBuf[20];

#if 0
void p_uart1_init()
{
	SCON = 0x50;		                      // 8位数据，可变波特率
	AUXR |= 0x40;		                      // 定时器1时钟1为FOSC,即1T
	AUXR &= 0xFE;		                      // 串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;		                      // 设定定时器1工作模式0 为16位自动重装方式
	TL1 = (65536 - (FOSC / 4 / BAUD));        // 设置定时初值 这里设置12为晶振
	TH1 = (65536 - (FOSC / 4 / BAUD)) >> 8;   // 设置定时初值      		                   
	TR1 = 1;		                          // 启动定时器1
	enableUart1Interrupts();
	//enableExtiInterrupts();
	memset(&COM1, 0, sizeof(COM1));
}



void TX1_write2buff(u8 dat)	//写入发送缓冲，指针+1
{
	TX1_Buffer[COM1.TX_write] = dat;	//装发送缓冲
	if (++COM1.TX_write >= COM_TX1_Lenth)
		COM1.TX_write = 0;

	if (COM1.B_TX_busy == 0)		//空闲
	{
		COM1.B_TX_busy = 1;		//标志忙
		TI = 1;					//触发发送中断
	}
}


void PrintString1(u8 *put)
{
	while ((*put) != '\0')
	{
		//TX1_write2buff(*put);
		put++;
	}
}

#endif


void p_uart2_init(void)
{
	/* 开串口2 */
	S2CON = 0x50;		                      // 8位数据，可变波特率
	AUXR |= 0x04;		                      // 定时器2时钟1为FOSC,即1T
	T2L = (65536 - (FOSC / 4 / BAUD));        // 设置定时初值 这里设置12为晶振
	T2H = (65536 - (FOSC / 4 / BAUD)) >> 8;   // 设置定时初值
	AUXR |= 0x10;		                      // 启动定时器2	      	
	enableUart2Interrupts();
	memset(&COM2, 0, sizeof(COM2));
}

void TX2_send_data(u8 *payload_dat, u16 payload_len)
{
	u16 i = 0;
	for (i = 0; i < payload_len; i++)
	{
		TX2_write2buff(payload_dat[i]);

		//发送也打印
		//TX1_write2buff(payload_dat[i]);
	}
}

void TX2_write2buff(u8 dat)	//写入发送缓冲，指针+1
{
	TX2_Buffer[COM2.TX_write] = dat;	//装发送缓冲
	if (++COM2.TX_write >= COM_TX2_Lenth)	COM2.TX_write = 0;

	if (COM2.B_TX_busy == 0)		//空闲
	{
		COM2.B_TX_busy = 1;		//标志忙
		//SET_TI2();				//触发发送中断
		S2CON |= (1 << 1);
	}
}


#if 0
void Uart1() interrupt 4 using 1
{
	if (RI)
	{
		RI = 0;
		if (COM1.B_RX_OK == 0)
		{
			if (COM1.RX_Cnt >= COM_RX1_Lenth)
				COM1.RX_Cnt = 0;
			RX1_Buffer[COM1.RX_Cnt] = SBUF;
			//透传  --- 用于电脑和通讯模块进行通信
			//TX2_write2buff(RX1_Buffer[COM1.RX_Cnt]);
			COM1.RX_Cnt++;
			COM1.RX_TimeOut = TimeOutSet1;
		}
	}

	if (TI)
	{
		TI = 0;
		if (COM1.TX_read != COM1.TX_write)
		{
			SBUF = TX1_Buffer[COM1.TX_read];
			if (++COM1.TX_read >= COM_TX1_Lenth)
				COM1.TX_read = 0;
		}
		else	COM1.B_TX_busy = 0;
	}

}
#endif 

/* 串口2中断  <--> GSM/LTE */
void Uart2() interrupt 8 using 1
{
	//if (RI2)
	if (S2CON & 0x01)
	{
		bFreeTimeOut1s = 5;

		//CLR_RI2();
		S2CON &= ~0x01;
		if (COM2.B_RX_OK == 0)
		{
			if (COM2.RX_Cnt >= COM_RX2_Lenth)	COM2.RX_Cnt = 0;
			RX2_Buffer[COM2.RX_Cnt] = S2BUF;

			// 打印串口2接收的数据
			//TX1_write2buff(RX2_Buffer[COM2.RX_Cnt]);
			COM2.RX_Cnt++;
			COM2.RX_TimeOut = TimeOutSet2;

			//判断一下接收头
			if ((RX2_Buffer[0] != 0xff) || ((COM2.RX_Cnt > 1) && (RX2_Buffer[1] != 0xaa)))
			{
				COM2.RX_Cnt = 0;
			}

		}
	}

	//if (TI2)
    if (S2CON & 0x02)
	{
		bFreeTimeOut1s = 5;

		//CLR_TI2();
		S2CON &= ~0x02;
		if (COM2.TX_read != COM2.TX_write)
		{
			S2BUF = TX2_Buffer[COM2.TX_read];
			if (++COM2.TX_read >= COM_TX2_Lenth)		COM2.TX_read = 0;
		}
		else
		{
			COM2.B_TX_busy = 0;
		}
	}

}

/* 接收发送一体函数 */
void pUart_Pro(void)
{
	//	static u8 bTrySend = 0;

		//发送数据
	if (COM2.TX_TimeOut == 0)
	{
#if 0
		if (g_isCommWait)
		{
			//心跳2次不正常则视为S2休眠
			if ((bTxBuf[4] == 0x01) && (bTrySend > 0))
			{
				g_isCommWait = 0;
				if (g_isCommErr == 0)
				{
					g_isCommErr = 1;
					g_bPlusTimeOut1s = 2;
				}
				g_isCommTry = 0;
				bTrySend = 0;
			}
			else
			{
				bTrySend++;

				if (bTrySend > 1)
				{
					if (g_isCommErr == 0)
					{
						g_isCommErr = 1;
						g_bPlusTimeOut1s = 2;
					}

				}

				if (bTrySend > 5)
				{
					bTrySend = 0;
					//重发超过5次  (5s) 则抛弃
					g_isCommWait = 0;

					g_isCommTry = 0;
				}
				else
				{
					g_isCommTry = 1;
				}
			}
		}
		else
		{
			bTrySend = 0;
		}
#endif
		pUart_Send_Pro();
	}

	//接收解析数据
	if (COM2.RX_TimeOut == 0)
	{
		if (COM2.RX_Cnt > 0)
		{
			COM2.B_RX_OK = 1;

			p_appc_proc_data(&RX2_Buffer, COM2.RX_Cnt);

			COM2.RX_Cnt = 0;
			COM2.B_RX_OK = 0;
		}

	}
}
/* 从解析数据中获取灯的数据 */
void LED_Control(u8 iLED_Type, u8 iLED_Value)
{
	switch (iLED_Type)
	{
	case '1':					                // 报警LED
		if (iLED_Value == '0') {				// 熄灭
			g_bLedSOS = LED_OFF;

		}
		else if (iLED_Value == '1') {			// 常亮
			g_bLedSOS = LED_ON;
		}
		else if (iLED_Value == '2') {			// 快闪
			g_bLedSOS = LED_FLASH_FAST;
		}
		else if (iLED_Value == '3') {			// 慢闪
			g_bLedSOS = LED_FLASH;
		}
		else {

		}
		break;
	case '2':					                // 运行
		if (iLED_Value == '0') {				// 熄灭
			g_bLedRun = LED_OFF;
		}
		else if (iLED_Value == '1') {			// 常亮
			g_bLedRun = LED_ON;
		}
		else if (iLED_Value == '2') {			// 快闪
			g_bLedRun = LED_FLASH_FAST;
		}
		else if (iLED_Value == '3') {			// 慢闪
			g_bLedRun = LED_FLASH;
		}
		else {

		}
		break;
#if 0
	case '3':					                // 充电
		if (iLED_Value == '0') {				// 熄灭
			g_bLedLamp = LED_OFF;
		}
		else if (iLED_Value == '1') {			// 常亮
			g_bLedLamp = LED_ON;
		}
		else if (iLED_Value == '2') {			// 快闪
			g_bLedLamp = LED_FLASH_FAST;
		}
		else if (iLED_Value == '3') {			// 慢闪
			g_bLedLamp = LED_FLASH;
		}
		break;
#endif
	default:
		break;
	}
}
/* 计算检验数 */
u8 pSum(u8 *bData, u8 bLen)
{
	u8 i, bTemp;

	bTemp = 0;

	for (i = 0; i < bLen - 1; i++)
	{
		bTemp += bData[i];
	}

	bTemp = ~bTemp;

	return bTemp;
}
/* 接收数据的返还值 */
void pUartReturn(u8 *bData, u8 bLen)
{
	u8 xdata bRTxBuf[20];
	memcpy(bRTxBuf, bData, bLen);
	bRTxBuf[3] |= 0x80;   // 功能高位置1
	//bRTxBuf[4] |= 0x80;   
	bRTxBuf[4] &= 0x0f;   // 功能高位置0
	bRTxBuf[bLen - 1] = pSum(bRTxBuf, bLen);
	TX2_send_data(bRTxBuf, bLen);

}
/* 解析模块数据 */
void p_appc_proc_data(u8 *bData, u8 bLength)
{
	u8 i;
	u8 bLen;
	//校验头
	if ((bData[0] != 0xff) || (bData[1] != 0xaa))
	{
		return;
	}

	//校验长度
	bLen = bData[5] + 7;
	if (bLen > bLength)
	{
		return;
	}
	//校验
	if (bData[bLen - 1] != pSum(bData, bLen))
	{
		return;
	}

	g_isCommErr = 0;	 		//通讯正常
	g_bCommErrTimeOut01s = 0;
	switch (bData[4])
	{
		
	  /* 回复(发送心跳和按键的回复值) */
	case 0x81:
	case 0x82:
	/* 充电状态的回复 */
	case 0x83:
		if (bData[2] == g_bSendNum)
		{
			g_isCommErr = 0;		//通讯OK
			g_isCommWait = 0;
			g_isCommTry = 0;
			//g_bPowerUpTimeOut1s = 0;
			//PrintString1("Rcv Ok\r\n");
		}
		break;
		
      /*  解析  */
		//功放控制
#if 0 	
	case 0x04:
		if (bData[6] == 0)
		{
			//IO_PA_SHAN = 0;
			PrintString1("AP0\r\n");
		}
		else if (bData[6] == 1)
		{
			//IO_PA_SHAN = 1;
			PrintString1("AP1\r\n");
		}
		//回复
		pUartReturn(bData, bLen);
		break;
#endif
		//灯控制
	//case 0x05:
	case 0x06:
		//PrintString1("LED Ctrl");
		for (i = 0; i < 3; i++)
		{
			//TX1_write2buff(bData[6 + i] + '0');
			LED_Control(i + '1', bData[6 + i] + '0');
		}
		//PrintString1("\r\n");
		g_isLedBak = 0;
		// 回复
		pUartReturn(bData, bLen);
		break;
		//电源控制
#if 0
	case 0x06:
		if (bData[6] == 1)
		{
			PrintString1("PWR1\r\n");
			//重上电
			g_isS2Reset = 1;
		}
		else if (bData[6] == 2)
		{
			//断电
			PrintString1("PWR2\r\n");
			g_isS2Off = 1;
		}
		//回复
		pUartReturn(bData, bLen);
		break;
#endif 
		// 熄屏
	case 0x07:
		//g_isS2CommSleep = 1;
		//PrintString1("ComSleep\r\n");
		// 回复
		pUartReturn(bData, bLen);
		break;
		// 亮屏
	case 0x08:
		//g_isS2CommSleep = 0;
		//PrintString1("ComWake\r\n");
		// 回复
		pUartReturn(bData, bLen);
		break;
	default:
		break;
	}
}
/* 发送给模块数据 */
void pUart_Send_Pro(void)
{
#if 0
	if (g_isCommTry)
	{
		if ((bTxBuf[4] != 0x01))
		{
			if (g_isS2CommSleep&&g_isCommErr)
			{
				PrintString1("Wake\r\n");
				GSM_PWRKEY = 0;
				Delay_nms(100);
				GSM_PWRKEY = 1;
				Delay_nms(100);
			}

		}

		PrintString1("Send ReTry\r\n");

		TX2_send_data(bTxBuf, bTxLen);
		COM2.TX_TimeOut = 50;		//1s超时回复
		g_isCommWait = 1;			//等待回复
	}
#endif
	if (g_isCommTry)
	{

	}

	//有按键则发按键  --- 按键信息
	else if ((g_bKey[0] != 0) && (g_bKey[0] != 0x86))
	{
#if 0
		// 这里是在获取设备息屏时将设备进行开机进行设备通讯
		if (g_isS2CommSleep&&g_isCommErr)
		{
			PrintString1("Wake\r\n");
			GSM_PWRKEY = 0;
			Delay_nms(100);
			GSM_PWRKEY = 1;
			Delay_nms(100);
		}
#endif
		g_bSendNum++;
		bTxBuf[0] = 0xff;
		bTxBuf[1] = 0xaa;
		bTxBuf[2] = g_bSendNum;
		bTxBuf[3] = 0x01;
		bTxBuf[4] = 0x02;	            // 按键功能
		bTxBuf[5] = 2;                  // 命令长度
		bTxBuf[6] = g_bKey[0] & 0x0f;   // 具体的功能（哪个按键和按键）
		if ((g_bKey[0] & 0x80) != 0)
		{
			bTxBuf[7] = 2;
		}
		else
		{
			bTxBuf[7] = 1;
		}
		bTxLen = bTxBuf[5] + 7;
		bTxBuf[8] = pSum(bTxBuf, bTxLen);// 校验位
		TX2_send_data(bTxBuf, bTxLen);
		COM2.TX_TimeOut = 50;		     // 1s超时回复
		g_isCommWait = 1;		         // 等待回复
		//清除按键
		if (g_bKey[1] != 0)
		{
			g_bKey[0] = g_bKey[1];
			g_bKey[1] = 0;
		}
		else
		{
			g_bKey[0] = 0;
		}
	}
#if 0
	//交流电
	else if (g_bPower != 0)
	{
		if (g_isS2CommSleep&&g_isCommErr)
		{
			PrintString1("Wake\r\n");
			GSM_PWRKEY = 0;
			Delay_nms(100);
			GSM_PWRKEY = 1;
			Delay_nms(100);
		}

		g_bSendNum++;
		bTxBuf[0] = 0xff;
		bTxBuf[1] = 0xaa;
		bTxBuf[2] = g_bSendNum;
		bTxBuf[3] = 0x01;
		bTxBuf[4] = 0x03;	//cmd
		bTxBuf[5] = 1;
		bTxBuf[6] = g_bPower - '0';

		bTxLen = bTxBuf[5] + 7;
		bTxBuf[7] = pSum(bTxBuf, bTxLen);
		TX2_send_data(bTxBuf, bTxLen);
		COM2.TX_TimeOut = 50;		//1s超时回复
		g_isCommWait = 1;		//等待回复

		g_bPower = 0;
	}
#endif

	//定时发送  --- 心跳包
	else if ((g_bPlusTimeOut1s == 0)/*||(g_isCommErr)*/)
	{
		g_bSendNum++;
		//PrintString1("Send Plus\r\n");
#if 0
		if (g_bPowerUpTimeOut1s > 0)
		{
			g_bPlusTimeOut1s = 2;
		}
#endif
		//else if (g_isCommErr)
		if (g_isCommErr)
		{
			g_bPlusTimeOut1s = 60;
		}
		else
		{
			g_bPlusTimeOut1s = 2;
		}
		bTxBuf[0] = 0xff;
		bTxBuf[1] = 0xaa;
		bTxBuf[2] = g_bSendNum;
		bTxBuf[3] = 0x01;
		bTxBuf[4] = 0x01;	//cmd
		bTxBuf[5] = 0;

		bTxLen = bTxBuf[5] + 7;
		bTxBuf[6] = pSum(bTxBuf, bTxLen);
		TX2_send_data(bTxBuf, bTxLen);
		COM2.TX_TimeOut = 50;
		g_isCommWait = 1;		//等待回复
	}
}




