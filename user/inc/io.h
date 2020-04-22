#ifndef _IO_H_
#define _IO_H_
#include "Config.h"

//LED
sbit IO_LED_RUN = P3 ^ 4;		// ����ָʾ
sbit IO_LED_SOS = P5 ^ 5;		// ���ָʾ

#if 0
/*Ӳ���ӿڶ���*/
sbit IO_DECT_AC = P3 ^ 7;		// �����
sbit IO_DECT_CHG = P2 ^ 4;		// �������
sbit IO_PA_SHAN = P0 ^ 2;		// �˷�ʹ��
sbit IO_RXD2 = P1 ^ 0;
//LED
sbit IO_LED_RUN = P3 ^ 4;		// ����ָʾ
sbit IO_LED_QJ = P3 ^ 5;		// ���ָʾ
sbit IO_LED_PWRIN = P2 ^ 1;	// ���ָʾ
sbit IO_LED_LAMP1 = P2 ^ 2;	// �ֵ�Ͳ
sbit IO_LED_LAMP2 = P2 ^ 3;
#endif
//��Դ
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

/*�ⲿ��������*/
void IO_Init(void);								
void Check_AC_In(void);						
void LED_pro(void);
void GPIO_SavePower(void);
#endif

