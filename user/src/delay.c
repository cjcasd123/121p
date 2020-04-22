#include "delay.h"
 
volatile unsigned long g_time_ms;

// g_time_ms(�����Ҫ�����޸�)
void Delay_nms(uint32_t ms)
{
	ms += my_time();
	while (ms > my_time());
}
void Delay_100ms(uint8_t iCnt)
{
	while (iCnt--)
	{
		Delay_nms(100);
	}
}


#if 0
void Delay_nms(uint8_t iCnt)
{
	uint16_t i = 0;

	do {	// 20191106:ʵ��11.0592M��14000ƫ��̫��10000��ࣻ		
		i = FOSC / 10000;			// 14T per loop �� 14 ��0.0904us(11.0592M) �� 1.2659us
		while (--i);   				// 14T per loop �� 14 ��0.0452us(22.1184M) �� 0.6328us	
	} while (--iCnt);
}
void Delay_100ms(uint8 iCnt)
{
	while (iCnt--)
	{
		Delay_nms(100);
	}
}
#endif


/*  ������ϵͳʱ�ӣ�����12MHZ */
void p_sysclk_init(void)
{
	/*ѡ���ڲ�24MHz�߾���IRC*/
	P_SW2 = 0x80;
	CKSEL = 0x00;                               //ѡ���ڲ�IRC ( Ĭ�� )
	CLKDIV = 0x02;								// 2��Ƶ 24MHz/2 = 12Mhz
	P_SW2 = 0x00;
}

/* 1ms ʮ��λ�Զ����� 12T */
void p_timer_init(void)
{
	/* Timer0 ��ʼ�� ʱ���10ms */
	AUXR &= 0x7F;		// ��ʱ��ʱ��12Tģʽ
	TMOD &= 0xF0;		// ���ö�ʱ��ģʽ
	TL0 = 0x18;			// ���ö�ʱ��ֵ
	TH0 = 0xfc;			// ���ö�ʱ��ֵ
	TF0 = 0;			// ���TF0��־
	TR0 = 1;			// ��ʱ��0��ʼ��ʱ

	/* ʹ�ܶ�ʱ���ж� */
	ET0 = 1;	                   // ʹ�ܶ�ʱ��0�ж�
	enableExtiInterrupts();
}















