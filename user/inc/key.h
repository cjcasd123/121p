#ifndef _KEY_H_
#define _KEY_H_


/* 目前的IO口 */
sbit KEY1 = P3 ^ 2;    // SOS
sbit KEY2 = P3 ^ 3;    // CALL

#if 0
/*硬件定义*/
sbit KEY1 = P1 ^ 4;
sbit KEY2 = P1 ^ 3;
sbit KEY3 = P1 ^ 6;
sbit KEY4 = P1 ^ 5;
sbit KEY5 = P1 ^ 2;
sbit KEY6 = P3 ^ 2;		//POWER
#endif
/*全局变量声明*/
extern u8 xdata g_bKey[2];
extern u8 xdata bKeyTimeOut1s;
extern u8 sign_date;
/*外部函数声明*/
void pAddKey(u8 bWhich, bit isTouch);
bit pSleepAble(void);
#endif

