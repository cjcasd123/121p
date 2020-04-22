#ifndef __DELAY_H__
#define __DELAY_H__
#include "config.h"



extern volatile unsigned long g_time_ms;
//void p_delay_10ms(uint32_t ms);
void p_sysclk_init(void);
void p_timer_init(void);
void Delay_nms(uint32_t ms);				
void Delay_100ms(uint8_t iCnt);

#endif  