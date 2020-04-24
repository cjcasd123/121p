#include "Config.h"


u8 sign_date;
u8 xdata g_bKey[2] = { 0,0 };	//最多存储2个按键缓冲
u8 xdata bKeyTimeOut1s;
/*  定时器(20MS进入一次) -- 发送 --第几个按键被按下  -- 属于常按还是短按 */
void pAddKey(u8 bWhich, bit isTouch)
{
	static u8 xdata bKeyCnt[6];

	u8 bPos = 0;
#if 0
	//第2个按键可以被更新，有可能关机键
	if (g_bKey[0] != 0)
	{
		bPos = 1;
	}
	else
	{
		bPos = 0;
	}
#endif
	if (isTouch)
	{
		bFreeTimeOut1s = 5;
		/* 累加按键按下的时间 */
		if (bKeyCnt[bWhich] < 0xff)
		{
			bKeyCnt[bWhich]++;
		}

		//长按2s   --- 20*100 = 2S
		if (bKeyCnt[bWhich] == 100)
		{
			//sign_date = 5;
			g_bKey[bPos] = 0x81 + bWhich;
			//TX1_write2buff('0' + bPos);
			//PrintString1("L_KEY:");
			//TX1_write2buff(bWhich + '0');
			bKeyTimeOut1s = 5;	//5s抛弃
		}
	}
	else   // 不按了即为高电平
	{
		//短按（大于40MS，小于2S）
		if ((bKeyCnt[bWhich] > 2) && (bKeyCnt[bWhich] < 100))
		{
			//sign_date = 5;
			g_bKey[bPos] = 0x01 + bWhich;
			//TX1_write2buff('0' + bPos);
			//PrintString1("KEY:");
			//TX1_write2buff(bWhich + '0');
			bKeyTimeOut1s = 5;	//5s抛弃
		}
		bKeyCnt[bWhich] = 0;   // 清空计算按键的时间
	}
}


/* 识别是否有按键被按下 ---返回为1即是进入睡眠  */
bit pSleepAble(void)
{
	if (bFreeTimeOut1s > 0)
	{
		return 0;
	}

	if ((KEY1 == 0))
	{
		bFreeTimeOut1s = 5;
		return 0;
	}

	if ((KEY2 == 0))
	{
		bFreeTimeOut1s = 5;
		return 0;
	}
#if 0
	if ((KEY3 == 0))
	{
		bFreeTimeOut1s = 5;
		return 0;
	}
	if ((KEY4 == 0))
	{
		bFreeTimeOut1s = 5;
		return 0;
	}
	if ((KEY5 == 0))
	{
		bFreeTimeOut1s = 5;
		return 0;
	}
	if ((KEY6 == 0))
	{
		bFreeTimeOut1s = 5;
		return 0;
	}

#endif
	return 1;
}



