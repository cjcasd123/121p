#include "delay.h"
 
volatile unsigned long g_time_ms;

// g_time_ms(这个需要进行修改)
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

	do {	// 20191106:实测11.0592M，14000偏差太大，10000差不多；		
		i = FOSC / 10000;			// 14T per loop ＝ 14 ×0.0904us(11.0592M) ＝ 1.2659us
		while (--i);   				// 14T per loop ＝ 14 ×0.0452us(22.1184M) ＝ 0.6328us	
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


/*  这里是系统时钟（晶振）12MHZ */
void p_sysclk_init(void)
{
	/*选择内部24MHz高精度IRC*/
	P_SW2 = 0x80;
	CKSEL = 0x00;                               //选择内部IRC ( 默认 )
	CLKDIV = 0x02;								// 2分频 24MHz/2 = 12Mhz
	P_SW2 = 0x00;
}

/* 1ms 十六位自动重载 12T */
void p_timer_init(void)
{
	/* Timer0 初始化 时间戳10ms */
	AUXR &= 0x7F;		// 定时器时钟12T模式
	TMOD &= 0xF0;		// 设置定时器模式
	TL0 = 0x18;			// 设置定时初值
	TH0 = 0xfc;			// 设置定时初值
	TF0 = 0;			// 清除TF0标志
	TR0 = 1;			// 定时器0开始计时

	/* 使能定时器中断 */
	ET0 = 1;	                   // 使能定时器0中断
	enableExtiInterrupts();
}















