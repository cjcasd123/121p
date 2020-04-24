#include "gsm.h"



//开机参数
void ResetPara(void)
{
	/* 通信和是否睡眠 */
	g_isCommErr = 1;   // 不通信
	g_isS2Sleep = 0;   // 不睡眠
	/* 心跳包的初始时间 */
//	bFreeTimeOut1s = 5;
	g_bPlusTimeOut1s = 120;
	/* 灯和按键值初始化 */
	g_bKey[0] = 0;
	g_bKey[1] = 0;
	g_bLedRun = 0;
	g_bLedSOS = 0;
}










