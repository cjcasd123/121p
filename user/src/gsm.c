#include "gsm.h"



//��������
void ResetPara(void)
{
	/* ͨ�ź��Ƿ�˯�� */
	g_isCommErr = 1;   // ��ͨ��
	g_isS2Sleep = 0;   // ��˯��
	/* �������ĳ�ʼʱ�� */
//	bFreeTimeOut1s = 5;
	g_bPlusTimeOut1s = 120;
	/* �ƺͰ���ֵ��ʼ�� */
	g_bKey[0] = 0;
	g_bKey[1] = 0;
	g_bLedRun = 0;
	g_bLedSOS = 0;
}










