#ifndef __USART_H__
#define __USART_H__
#include "config.h"

#define FOSC 12000000
#define BAUD 9600

#define	COM_TX1_Lenth		50
#define	COM_RX1_Lenth		50

#define	COM_TX2_Lenth		50		
#define	COM_RX2_Lenth		100


typedef struct
{
	u8 id;
	u16	TX_read;		//发送读指针
	u16	TX_write;		//发送写指针
	u8	B_TX_busy;		//忙标志

	u8  TX_TimeOut;		//发送超时

	u16 RX_Cnt;			//接收字节计数
	u8	RX_TimeOut;		//接收超时
	u8	B_RX_OK;		//接收块完成
} COMx_Define;

//#if 0
extern	COMx_Define	 xdata COM1;
extern	u8 xdata	 TX1_Buffer[COM_TX1_Lenth];	//发送缓冲
extern	u8 xdata 	 RX1_Buffer[COM_RX1_Lenth];	//接收缓冲
//#endif
extern	COMx_Define	 xdata COM2;
extern	u8	xdata TX2_Buffer[COM_TX2_Lenth];	//发送缓冲
extern	u8 	xdata RX2_Buffer[COM_RX2_Lenth];	//接收缓冲

extern bit g_isCommWait, g_isCommErr, g_isCommTry;
extern u8 g_bCommErrTimeOut01s;
extern u8 g_bSendNum;

//#if 0
void p_uart1_init(void);
void TX1_send_data(u8 *payload_dat, u16 payload_len);
void TX1_write2buff(u8 dat);	//写入发送缓冲，指针+1
void PrintString1(u8 *put);
//#endif
void p_uart2_init(void);
void TX2_send_data(u8 *payload_dat, u16 payload_len);
void TX2_write2buff(u8 dat);	//写入发送缓冲，指针+1
//void PrintString2(u8 *puts);

// 解析函数
void pUart_Pro(void);
void LED_Control(u8 iLED_Type, u8 iLED_Value);
u8 pSum(u8 *bData, u8 bLen);
void pUartReturn(u8 *bData, u8 bLen);
void p_appc_proc_data(u8 *bData, u8 bLength);
void pUart_Send_Pro(void);
#endif  