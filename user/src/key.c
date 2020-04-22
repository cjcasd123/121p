#include "Config.h"


u8 sign_date;
u8 xdata g_bKey[2] = { 0,0 };	//���洢2����������
u8 xdata bKeyTimeOut1s;
/*  ��ʱ��(20MS����һ��) -- ���� --�ڼ�������������  -- ���ڳ������Ƕ̰� */
void pAddKey(u8 bWhich, bit isTouch)
{
	static u8 xdata bKeyCnt[6];

	u8 bPos;

	//��2���������Ա����£��п��ܹػ���
	if (g_bKey[0] != 0)
	{
		bPos = 1;
	}
	else
	{
		bPos = 0;
	}

	if (isTouch)
	{
		bFreeTimeOut1s = 5;
		/* ������һ����������ʱ��Ĳ��� */
		if (bKeyCnt[bWhich] < 0xff)
		{
			bKeyCnt[bWhich]++;
		}

		//����2s
		if (bKeyCnt[bWhich] == 100)
		{

			sign_date = 5;

			g_bKey[bPos] = 0x81 + bWhich;

			//TX1_write2buff('0' + bPos);
			//PrintString1("L_KEY:");

			//TX1_write2buff(bWhich + '0');

			bKeyTimeOut1s = 5;	//5s����
		}
	}
	else   // �����˼�Ϊ�ߵ�ƽ
	{
		//�̰������ڣ�С��2S��
		if ((bKeyCnt[bWhich] > 2) && (bKeyCnt[bWhich] < 100))
		{


			sign_date = 5;
			g_bKey[bPos] = 0x01 + bWhich;

			//TX1_write2buff('0' + bPos);
			//PrintString1("KEY:");
			//TX1_write2buff(bWhich + '0');
			bKeyTimeOut1s = 5;	//5s����
		}
		bKeyCnt[bWhich] = 0;   // ���
	}
}


/* ʶ���Ƿ��а��������� ---����Ϊ1���ǽ���˯��  */
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



