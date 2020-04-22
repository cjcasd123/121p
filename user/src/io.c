#include "Config.h"

// ��©�����	PXM1 H   PXM0 H		// �ڲ������Ͽ�	����ʡ��
// �������룺	PXM1 H   PXM0 L		// �����Ȳ����룬Ҳ������������ʡ��
// ����˫��	PXM1 L   PXM0 L		// ��������������ɴ�20mA��������270uA 
// ���������	PXM1 L   PXM0 H		// �ɴ�20mA������������裻��оƬ��������/��������������90mA��


// ��Դ
//u8 xdata g_bPower;
//u8 xdata iPWR_In;
// LED
u8 xdata g_bLedRun, g_bLedSOS, g_bLedLamp, g_bLedCd;
//u8 xdata g_bLedRunBak, g_bLedQjBak, g_bLedLampBak;
bit g_isLedBak;
bit isFlash, isFlashFast;



/* ���ݱ���ʵ��ִ��IO�ڵĵ� */
void pLedRun()    
{

	//���е�
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

	//���
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
	//����
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

	//������
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

	//���ָʾ��
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
		/* ���������һ�û����ͨ��  */
		if (g_isCommErr == 1)
			g_bLedRun = 1;   
        // ��Ϊ0ʱ����Ƶ��
	}
	/* Ϣ��ʱ�ص� */
	else
	{
		g_bLedSOS = 0;
		g_bLedRun = 0;
	}
	pLedRun();
}

#if 0
/* ����ָʾ�ƵĲ����� */
void LED_pro(void)  
{
	static u8 bLastTime;
	static bit isUpFlash;


	// �������ʱ�ģ���������
	u8 g_bPowerUpTimeOut1s, g_bPowerDownTimeOut1s;
	//����ָʾ������
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

	//�ػ�ָʾ�����ã�׼���رգ�
	else if (g_bPowerDownTimeOut1s > 0)
	{
		g_bLedRun = LED_ON;
		g_bLedSOS = LED_ON;
		g_bLedLamp = LED_OFF;
	}

	//�ػ���ȫ��ָʾ������Ĺرգ�
	else if (g_isS2Sleep)
	{
		g_bLedRun = LED_OFF;
		g_bLedSOS = LED_OFF;
		g_bLedLamp = LED_OFF;
	}
	else       // �豸����ʱ����ʾ��
	{
		//ͨѶ���ϣ��������ݲ�ȫ����
		if (g_isCommErr)
		{
			//ͨѶ������Ҫ����LED״̬
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
		// ����ͨѶ��ȫ������
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
/* IO�ڳ�ʼ�� */
void IO_Init(void)  // һ��Ĭ����׼˫��Ĺ����ڲ������޸�   ---- �����ر�ע��usb_boot�ܽţ������һ��Ϊ��©���ͣ�����������
{
	//δʹ�ùܽţ�����׼˫��ʡ��
	P0M0 = P0M1 = 0;
	P1M0 = P1M1 = 0;
	P2M0 = P2M1 = 0;
	P3M0 = P3M1 = 0;
	P4M0 = P4M1 = 0;

	//LED���ƹܽų�ʼ��--׼˫��
	P3M1 &= ~(1 << 4);			// ����
	P3M0 &= ~(1 << 4);
	P3M1 &= ~(1 << 5);			// ���
	P3M0 &= ~(1 << 5);

//	P2M1 &= ~(1 << 1);			// ���
//	P2M0 &= ~(1 << 1);

	// �ֵ�Ͳ�������˫���޲��
//	P2M1 &= ~((1 << 2) | (1 << 3));
//	P2M0 &= ~((1 << 2) | (1 << 3));

	//KEY�ܽų�ʼ��P3.2/3--׼˫�� 
	P3M1 &= ~((1 << 2) | (1 << 3));
	P3M0 &= ~((1 << 2) | (1 << 3));
//	P1M1 &= ~((1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6));
//	P1M0 &= ~((1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6));

#if 0
	// �����--���� 
	P2M1 |= (1 << 4);
	P2M0 &= ~(1 << 4);
	IO_DECT_AC = 0;
	// �������--����
	P3M1 |= (1 << 7);
	P3M0 &= ~(1 << 7);
	IO_DECT_CHG = 0;
	// ����ʹ��--����
	P0M1 &= ~(1 << 2);
	P0M0 |= (1 << 2);
	IO_PA_SHAN = 0;			// �ͽ�ֹ������ʹ��
#endif

}


/* IOʡ�紦�� */
void GPIO_SavePower(void)
{
	//GSM_READY = 0;						// S2ʵ����Ч�ܽţ��������ӵ���Ͽ���
	//GSM_PWRKEY = 1;						// ���Ϳ���	
	//GSM_RESET = 1;						// �͵�ƽ > 31.25ms ��λ


	// �����--���� 
	//IO_DECT_AC = 0;
	// �������--����
	//IO_DECT_CHG = 0;
	//IO_PA_SHAN = 0;			// �ͽ�ֹ������ʹ��

	g_bLedRun = LED_OFF;
	g_bLedCd = LED_OFF;
	g_bLedLamp = LED_OFF;
	g_bLedSOS = LED_OFF;
	IO_LED_RUN = OFF_LED;		// ����ָʾ
	IO_LED_SOS = OFF_LED;		// ���ָʾ
#if 0
	IO_LED_PWRIN = OFF_LED;		// ���ָʾ
	IO_LED_LAMP1 = OFF_LED;		// �ֵ�Ͳ
	IO_LED_LAMP2 = OFF_LED;
#endif
}

#if 0
/* ����� */
void Check_AC_In(void)
{
	// ���Ͽ���⡢�������
	if (bAC_In) {
		IO_DECT_AC = 1;
		if (!IO_DECT_AC) {		// �ߵ�ƽ���
			iDect_AC_Count++;
			if (iDect_AC_Count >= 3) {
				iDect_AC_Count = 0;
				bAC_In = 0;

				iPWR_In = 0;
				PrintString1("CHG OUT");
				g_bPower = '0';
			}
		}

		// �������
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
						iPWR_In = 1;		//תΪ�������״̬
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
		// �����	
	}
	else {
		IO_DECT_AC = 0;
		if (IO_DECT_AC) {		// �ߵ�ƽ���
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



