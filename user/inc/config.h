#ifndef __CONFIG_H__
#define __CONFIG_H__
/* Դ�ļ� */
//#include "STC8xxxx.h" 			
//#include "DATATYPE.h"
#include "stc8.h"
#include "type.h"
#include <intrins.h>
#include <absacc.h>
#include <math.h> 
#include <stdlib.h> 
#include <string.h>
/* �����ļ� */
#include "main.h"
#include "usart.h"
#include "IO.h" 
//#include "PR_Key.h"
#include "key.h"
#include "delay.h"
/* ǰ�������������͵�ת����int -- ASILL�� ���ֿ�����Ϊ�Ƕ����ݽ��еĽ����ж�  ��������ת���ɺ궨����д��  */
#define CUSTOM_ITOA_3(val, p) do {\
									(p)[2] = ((val) % 10)+'0'; (val) /= 10; \
									(p)[1] = ((val) % 10)+'0'; (val) /= 10; (p)[0] = ((val) % 10)+'0';\
								} while (0)

#define CUSTOM_ATOI_3(p)	(((p)[2]-'0')+((p)[1]-'0')*10+((p)[0]-'0')*100)

#define CHECK_VALUE(p)		((p)[0]>='0' && (p)[0]<='9' \
							&& (p)[1]>='0' && (p)[1]<='9' \
							&& (p)[2]>='0' && (p)[2]<='9')

#define CHECK_BOOL(p)		((p)[0]=='0' && (p)[1]=='0' \
							&& ((p)[2]=='0' || (p)[2] == '1'))
							
#define COMPARE_KEY(src, dst)	((src)[0] == (dst)[0] && (src)[1] == (dst)[1])



/*
     ������÷�    ע��IO�ڵ�ǰ��˳��
   1 PORT_MODE_SET(4, 3, PORT_MODE_PP);  ����P4.3�ڵ��������      
   2 PORT_PULL_UP_SET(4, 3);             ����P4.3�ں�����������
*/
#define PORT_MODE_AD			0x00	// ׼˫���
#define PORT_MODE_PP			0x01	// �������
#define PORT_MODE_HZ			0x02	// ��������
#define PORT_MODE_OD			0x03	// �������

/* ���ö˿ڵ�ģʽ */
#define PORT_MODE_SET(g, p, m)	_PORT_MODE_SET(g, p, m)
#define _PORT_MODE_SET(g, p, m)	do{\
									P##g##M0 |= ((m&0x01)<<(p)); \
									P##g##M1 |= ((m>>1)<<(p)); \
								}while(0)

/* ���ö˿��Ƿ���Ҫ��������3.7K, P3.0 P3.1��Сһ�� */
#define PORT_PULL_UP_SET(g, p)	_PORT_PULL_UP_SET(g, p)
#define _PORT_PULL_UP_SET(g, p)	do{P##g##PU |= (0x01<<(p));}while(0)






#endif
