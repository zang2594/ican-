#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"

#define KEY0  	PBin(9)		//读取按键0
#define KEY1  	PBin(8)   	//读取按键1
#define KEY2  	PCin(14)   	//读取按键2
#define KEY3  	PCin(15)
  
#define Body  	PAin(5)
#define QS  	PAin(6)
#define FY  	PAin(7) 

#define KEY0_PRES	1		//KEY0  
#define KEY1_PRES	2		//KEY1 
#define KEY2_PRES	3		//KEY2  
#define KEY3_PRES	4


void KEY_Init(void);//IO初始化
u8 KEY_Scan(u8 mode);  	//按键扫描函数	

#endif



