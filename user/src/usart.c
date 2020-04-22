#include "usart.h"

bit g_isCommWait, g_isCommErr, g_isCommTry;
u8 g_bCommErrTimeOut01s;
u8 g_bSendNum;

#if 0
COMx_Define	 xdata COM1;
u8	 xdata TX1_Buffer[COM_TX1_Lenth];	//���ͻ���
u8 	 xdata RX1_Buffer[COM_RX1_Lenth];	//���ջ���
#endif 
COMx_Define	 xdata COM2;
u8	xdata TX2_Buffer[COM_TX2_Lenth];	//���ͻ���
u8 	xdata RX2_Buffer[COM_RX2_Lenth];	//���ջ���

static u8 xdata bTxLen;
static u8 xdata bTxBuf[20];

#if 0
void p_uart1_init()
{
	SCON = 0x50;		                      // 8λ���ݣ��ɱ䲨����
	AUXR |= 0x40;		                      // ��ʱ��1ʱ��1ΪFOSC,��1T
	AUXR &= 0xFE;		                      // ����1ѡ��ʱ��1Ϊ�����ʷ�����
	TMOD &= 0x0F;		                      // �趨��ʱ��1����ģʽ0 Ϊ16λ�Զ���װ��ʽ
	TL1 = (65536 - (FOSC / 4 / BAUD));        // ���ö�ʱ��ֵ ��������12Ϊ����
	TH1 = (65536 - (FOSC / 4 / BAUD)) >> 8;   // ���ö�ʱ��ֵ      		                   
	TR1 = 1;		                          // ������ʱ��1
	enableUart1Interrupts();
	//enableExtiInterrupts();
	memset(&COM1, 0, sizeof(COM1));
}



void TX1_write2buff(u8 dat)	//д�뷢�ͻ��壬ָ��+1
{
	TX1_Buffer[COM1.TX_write] = dat;	//װ���ͻ���
	if (++COM1.TX_write >= COM_TX1_Lenth)
		COM1.TX_write = 0;

	if (COM1.B_TX_busy == 0)		//����
	{
		COM1.B_TX_busy = 1;		//��־æ
		TI = 1;					//���������ж�
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
	/* ������2 */
	S2CON = 0x50;		                      // 8λ���ݣ��ɱ䲨����
	AUXR |= 0x04;		                      // ��ʱ��2ʱ��1ΪFOSC,��1T
	T2L = (65536 - (FOSC / 4 / BAUD));        // ���ö�ʱ��ֵ ��������12Ϊ����
	T2H = (65536 - (FOSC / 4 / BAUD)) >> 8;   // ���ö�ʱ��ֵ
	AUXR |= 0x10;		                      // ������ʱ��2	      	
	enableUart2Interrupts();
	memset(&COM2, 0, sizeof(COM2));
}

void TX2_send_data(u8 *payload_dat, u16 payload_len)
{
	u16 i = 0;
	for (i = 0; i < payload_len; i++)
	{
		TX2_write2buff(payload_dat[i]);

		//����Ҳ��ӡ
		//TX1_write2buff(payload_dat[i]);
	}
}

void TX2_write2buff(u8 dat)	//д�뷢�ͻ��壬ָ��+1
{
	TX2_Buffer[COM2.TX_write] = dat;	//װ���ͻ���
	if (++COM2.TX_write >= COM_TX2_Lenth)	COM2.TX_write = 0;

	if (COM2.B_TX_busy == 0)		//����
	{
		COM2.B_TX_busy = 1;		//��־æ
		//SET_TI2();				//���������ж�
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
			//͸��  --- ���ڵ��Ժ�ͨѶģ�����ͨ��
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

/* ����2�ж�  <--> GSM/LTE */
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

			// ��ӡ����2���յ�����
			//TX1_write2buff(RX2_Buffer[COM2.RX_Cnt]);
			COM2.RX_Cnt++;
			COM2.RX_TimeOut = TimeOutSet2;

			//�ж�һ�½���ͷ
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

/* ���շ���һ�庯�� */
void pUart_Pro(void)
{
	//	static u8 bTrySend = 0;

		//��������
	if (COM2.TX_TimeOut == 0)
	{
#if 0
		if (g_isCommWait)
		{
			//����2�β���������ΪS2����
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
					//�ط�����5��  (5s) ������
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

	//���ս�������
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
/* �ӽ��������л�ȡ�Ƶ����� */
void LED_Control(u8 iLED_Type, u8 iLED_Value)
{
	switch (iLED_Type)
	{
	case '1':					                // ����LED
		if (iLED_Value == '0') {				// Ϩ��
			g_bLedSOS = LED_OFF;

		}
		else if (iLED_Value == '1') {			// ����
			g_bLedSOS = LED_ON;
		}
		else if (iLED_Value == '2') {			// ����
			g_bLedSOS = LED_FLASH_FAST;
		}
		else if (iLED_Value == '3') {			// ����
			g_bLedSOS = LED_FLASH;
		}
		else {

		}
		break;
	case '2':					                // ����
		if (iLED_Value == '0') {				// Ϩ��
			g_bLedRun = LED_OFF;
		}
		else if (iLED_Value == '1') {			// ����
			g_bLedRun = LED_ON;
		}
		else if (iLED_Value == '2') {			// ����
			g_bLedRun = LED_FLASH_FAST;
		}
		else if (iLED_Value == '3') {			// ����
			g_bLedRun = LED_FLASH;
		}
		else {

		}
		break;
#if 0
	case '3':					                // ���
		if (iLED_Value == '0') {				// Ϩ��
			g_bLedLamp = LED_OFF;
		}
		else if (iLED_Value == '1') {			// ����
			g_bLedLamp = LED_ON;
		}
		else if (iLED_Value == '2') {			// ����
			g_bLedLamp = LED_FLASH_FAST;
		}
		else if (iLED_Value == '3') {			// ����
			g_bLedLamp = LED_FLASH;
		}
		break;
#endif
	default:
		break;
	}
}
/* ��������� */
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
/* �������ݵķ���ֵ */
void pUartReturn(u8 *bData, u8 bLen)
{
	u8 xdata bRTxBuf[20];
	memcpy(bRTxBuf, bData, bLen);
	bRTxBuf[3] |= 0x80;   // ���ܸ�λ��1
	//bRTxBuf[4] |= 0x80;   
	bRTxBuf[4] &= 0x0f;   // ���ܸ�λ��0
	bRTxBuf[bLen - 1] = pSum(bRTxBuf, bLen);
	TX2_send_data(bRTxBuf, bLen);

}
/* ����ģ������ */
void p_appc_proc_data(u8 *bData, u8 bLength)
{
	u8 i;
	u8 bLen;
	//У��ͷ
	if ((bData[0] != 0xff) || (bData[1] != 0xaa))
	{
		return;
	}

	//У�鳤��
	bLen = bData[5] + 7;
	if (bLen > bLength)
	{
		return;
	}
	//У��
	if (bData[bLen - 1] != pSum(bData, bLen))
	{
		return;
	}

	g_isCommErr = 0;	 		//ͨѶ����
	g_bCommErrTimeOut01s = 0;
	switch (bData[4])
	{
		
	  /* �ظ�(���������Ͱ����Ļظ�ֵ) */
	case 0x81:
	case 0x82:
	/* ���״̬�Ļظ� */
	case 0x83:
		if (bData[2] == g_bSendNum)
		{
			g_isCommErr = 0;		//ͨѶOK
			g_isCommWait = 0;
			g_isCommTry = 0;
			//g_bPowerUpTimeOut1s = 0;
			//PrintString1("Rcv Ok\r\n");
		}
		break;
		
      /*  ����  */
		//���ſ���
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
		//�ظ�
		pUartReturn(bData, bLen);
		break;
#endif
		//�ƿ���
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
		// �ظ�
		pUartReturn(bData, bLen);
		break;
		//��Դ����
#if 0
	case 0x06:
		if (bData[6] == 1)
		{
			PrintString1("PWR1\r\n");
			//���ϵ�
			g_isS2Reset = 1;
		}
		else if (bData[6] == 2)
		{
			//�ϵ�
			PrintString1("PWR2\r\n");
			g_isS2Off = 1;
		}
		//�ظ�
		pUartReturn(bData, bLen);
		break;
#endif 
		// Ϩ��
	case 0x07:
		//g_isS2CommSleep = 1;
		//PrintString1("ComSleep\r\n");
		// �ظ�
		pUartReturn(bData, bLen);
		break;
		// ����
	case 0x08:
		//g_isS2CommSleep = 0;
		//PrintString1("ComWake\r\n");
		// �ظ�
		pUartReturn(bData, bLen);
		break;
	default:
		break;
	}
}
/* ���͸�ģ������ */
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
		COM2.TX_TimeOut = 50;		//1s��ʱ�ظ�
		g_isCommWait = 1;			//�ȴ��ظ�
	}
#endif
	if (g_isCommTry)
	{

	}

	//�а����򷢰���  --- ������Ϣ
	else if ((g_bKey[0] != 0) && (g_bKey[0] != 0x86))
	{
#if 0
		// �������ڻ�ȡ�豸Ϣ��ʱ���豸���п��������豸ͨѶ
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
		bTxBuf[4] = 0x02;	            // ��������
		bTxBuf[5] = 2;                  // �����
		bTxBuf[6] = g_bKey[0] & 0x0f;   // ����Ĺ��ܣ��ĸ������Ͱ�����
		if ((g_bKey[0] & 0x80) != 0)
		{
			bTxBuf[7] = 2;
		}
		else
		{
			bTxBuf[7] = 1;
		}
		bTxLen = bTxBuf[5] + 7;
		bTxBuf[8] = pSum(bTxBuf, bTxLen);// У��λ
		TX2_send_data(bTxBuf, bTxLen);
		COM2.TX_TimeOut = 50;		     // 1s��ʱ�ظ�
		g_isCommWait = 1;		         // �ȴ��ظ�
		//�������
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
	//������
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
		COM2.TX_TimeOut = 50;		//1s��ʱ�ظ�
		g_isCommWait = 1;		//�ȴ��ظ�

		g_bPower = 0;
	}
#endif

	//��ʱ����  --- ������
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
		g_isCommWait = 1;		//�ȴ��ظ�
	}
}




