#ifndef __CONFIG_H__
#define __CONFIG_H__
/* 源文件 */
//#include "STC8xxxx.h" 			
//#include "DATATYPE.h"
#include "stc8.h"
#include "type.h"
#include <intrins.h>
#include <absacc.h>
#include <math.h> 
#include <stdlib.h> 
#include <string.h>
/* 本地文件 */
#include "main.h"
#include "usart.h"
#include "IO.h" 
//#include "PR_Key.h"
#include "key.h"
#include "delay.h"
/* 前两种是数据类型的转换（int -- ASILL） 后几种可以认为是对数据进行的解析判断  （将函数转换成宏定义来写）  */
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
     具体的用法    注意IO口的前后顺序
   1 PORT_MODE_SET(4, 3, PORT_MODE_PP);  对于P4.3口的推挽输出      
   2 PORT_PULL_UP_SET(4, 3);             对于P4.3口含有上拉电阻
*/
#define PORT_MODE_AD			0x00	// 准双向扣
#define PORT_MODE_PP			0x01	// 推挽输出
#define PORT_MODE_HZ			0x02	// 高阻输入
#define PORT_MODE_OD			0x03	// 开口输出

/* 设置端口的模式 */
#define PORT_MODE_SET(g, p, m)	_PORT_MODE_SET(g, p, m)
#define _PORT_MODE_SET(g, p, m)	do{\
									P##g##M0 |= ((m&0x01)<<(p)); \
									P##g##M1 |= ((m>>1)<<(p)); \
								}while(0)

/* 设置端口是否需要上拉电阻3.7K, P3.0 P3.1会小一点 */
#define PORT_PULL_UP_SET(g, p)	_PORT_PULL_UP_SET(g, p)
#define _PORT_PULL_UP_SET(g, p)	do{P##g##PU |= (0x01<<(p));}while(0)






#endif
