#ifndef _GSM_H_
#define _GSM_H_
#include "config.h"

sbit usb_boot = P1 ^ 7;
sbit mcu_pkey = P1 ^ 6;
sbit c_lte = P3 ^ 5;
sbit wack_up = P3 ^ 7;


void ResetPara(void);




#endif


