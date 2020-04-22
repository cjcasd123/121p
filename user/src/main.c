#include "Config.h"

/*
       �ڲ���Դ˵��
  1.ʹ���ڲ�ʱ��  ---- 12MHZ
  2.ʹ�ô���1--��ӡ   ʹ�ô���2--ͨѶ   ������9600
  3.ʹ�ö�ʱ��0 --- 1MS�ж�

  �޸��˽��շ���ֵ���ֽ���1����0

  4.ͨ��Э��Ŀǰ��Ҫʱ���պͷ�����һ���
*/

sbit usb_boot = P1 ^ 7;
sbit mcu_pkey = P1 ^ 6;
sbit c_lte = P3 ^ 5;

/*ȫ�ֱ�������*/
volatile uint8_t iEvent_Flag = 0;					/* ϵͳ��־λ */
//volatile uint8 wWdog_1S_Timer = 0;
uint8_t iWdog_S = 0;
bit bAC_In = 0, bDect_Chg = 0;
u8  iDect_AC_Count = 0;
u8  iDect_Bat_Count = 0;

/*��̬��������*/
static uint8_t iTimer_Counter1 = 0;	  			// 100ms��ʱ
static uint8_t iTimer_Counter2 = 0;				// 1s��ʱ

u8  g_bPlusTimeOut1s;
u8  g_bSleepTimeOut1s;
u8  bFreeTimeOut1s;
bit g_isS2Sleep;
bit g_isS2On, g_isS2Off, g_isS2Reset;



//��������
void ResetPara(void)
{
	g_isCommErr = 1;
	g_isS2Sleep = 0;
	bFreeTimeOut1s = 5;
	g_bPlusTimeOut1s = 2;
	g_bKey[0] = 0;
	g_bKey[1] = 0;
}



/*������*/
void main(void)
{

	//  ������IO��������
	usb_boot = 0;
	// MCU��ʼ��						
	disableExtiInterrupts();
	p_sysclk_init();
	p_timer_init();
	//p_uart1_init();
	p_uart2_init();
	//IO_Init();   --- ����׼˫�����ʱ����
	//INT0_Init();
	enableExtiInterrupts();
	/* ����ģ�鲢���� */
	mcu_pkey = 0;
	c_lte = 0;
	Delay_100ms(40);
	// ���ؿ�����Ϊ��©���
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
	// ������������
	ResetPara();

	while (1)
	{

		//����5���׵�
		if ((bKeyTimeOut1s == 0) && ((g_bKey[0] != 0) || (g_bKey[1] != 0)))
		{
			g_bKey[0] = 0;
			g_bKey[1] = 0;
		}
#if 0
		// �ػ�  ���������¼�����������ģʽ�Ͷ�ʱ���ѣ�
		// ֱ�ӽ��йػ�   --- һ��ʼ������ػ�
		if (g_isS2Sleep)
		{
			// ����6����
			if ((g_bKey[0] == 0x86) || (g_bKey[1] == 0x86))
			{
				g_isS2On = 1;
				g_bKey[0] = 0;
				g_bKey[1] = 0;

				g_isS2Sleep = 0;
			}
			// IO�ڷ���ʼ��
			GPIO_SavePower();

			//�ֶ��ػ���ֻ����Դ��
			//�������ߣ��ް���5�룩����ʱ���ѣ���ν���ָʾ�ƣ� ��ο���ָʾ�ƣ���
			while (g_bSleepTimeOut1s == 0)
			{
				EX0 = 1;				// ���ⲿ�ж�
				PCON = 0x02;            // ��������ģʽ ��VOCTRL = 0x00;���������Ĭ�Ϲʿ��Բ�д 
				_nop_();
				_nop_();
				_nop_();
				_nop_();
				EX0 = 0;				//������ر��ⲿ�ж�
			}
			// IO�ڳ�ʼ��
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


		// �ػ������� --- û��ִ����������˯��
		//else
		//if (g_isS2Sleep == 1)   // �����޸���
		if (g_isS2Sleep == 0)
		{
			if ((g_bKey[0] == 0x86) || (g_bKey[1] == 0x86))
			{
				g_isS2Off = 1;
				g_bKey[0] = 0;
				g_bKey[1] = 0;
			}
  
			//MCU_IDLE();
			//û������������
			//���ߣ�100ms��ʱ����ִ��LED
			if (pSleepAble())
			{
				static u8 iSleep_Cnt;

				WakeTimerSet(160);			// 100ms�����綨ʱ����
				PCON = 0x02;
				_nop_();
				_nop_();
				_nop_();
				_nop_();

				isFlashFast = !isFlashFast;		//��ʱ
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
			//20���붨ʱ����	
			if (iEvent_Flag & EVENT_TIMER_100MS)
			{
				iEvent_Flag &= (uint8)(~EVENT_TIMER_100MS);
				// �豸�����
				//Check_AC_In();
				// ָʾ�Ʋ���������
				if (bFreeTimeOut1s == 0)
				{
					//LED_pro();
				}

				//  ͨѶ�Ƿ����� 
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
			/* ���շ��� */
			pUart_Pro();
		}
	}
}




/* ��ʱ��0 ʱ���  --- 1MS */
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
		/* ɨ���ĸ��������º�ʱ��ĳ��� */
		pAddKey(0, KEY1 == 0);
		pAddKey(1, KEY2 == 0);
		if (COM2.TX_TimeOut > 0)
		{
			COM2.TX_TimeOut--;
		}

		// 100�����ʱ
		iTimer_Counter1++;
		if (iTimer_Counter1 >= 5)   // 100����  1*20*5
		{
			iTimer_Counter1 = 0;

			iEvent_Flag |= EVENT_TIMER_100MS;

			isFlashFast = !isFlashFast;			//����

			// 1���ʱ   100*10
			iTimer_Counter2++;
			if (iTimer_Counter2 >= 10)
			{
				iTimer_Counter2 = 0;

				isFlash = !isFlash;					//����	
				/**********************************************/
				// ��ѯ�Ƿ��а�������
				if (bFreeTimeOut1s > 0)
				{
					bFreeTimeOut1s--;
				}
				// ����������ʱ
				if (g_bPlusTimeOut1s > 0)
				{
					g_bPlusTimeOut1s--;
				}
				// ��ʱ����
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
				// ���԰��������Ͷ̰�
				if (bKeyTimeOut1s > 0)
				{
					bKeyTimeOut1s--;
				}
				/**************************************************/

			}
		}
		//LED_pro();    // 20MS��ѯһ�ε�
	}

}








