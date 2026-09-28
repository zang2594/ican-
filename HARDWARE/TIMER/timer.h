#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"

void TIM3_Int_Init(u16 arr,u16 psc); 

extern u8 min,sec;
extern u8 js_flag;
extern u8 b1_flag;
extern u16 time_t;
extern u8 yy_flag; 
#endif

