#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "oled.h"  
#include "key.h"
#include "adc.h" 
#include "ds18b20.h"
#include "timer.h"
#include "usart3.h"  
#include "esp8266.h"
float Tur_ad;		//浑浊度值
int Tur_Value;
   
u8 moshi=0;	 
u16 time_t = 10;	//设置值
u16 temp_h = 35;
u16 ligh_l = 200;
u16 turb_h = 1000; 
short temperature; 
	int temp_n; 
	int adcx0,adcx1;
void Key_process()
{
	u8 t=0;
	u8 t1=0;
	
	t=KEY_Scan(0);		//得到键值
	t1=KEY_Scan(1);		//得到键值 
	
	if(t==KEY0_PRES)
	{
		OLED_Clear();
		moshi++;
		if(moshi>=5) moshi = 0;
		if(moshi==0)
		{
			OLED_ShowStr(16, 0,"定时:00:00 ",16);  
			OLED_ShowStr(16,16,"温度:  . ℃ ",16); 
			OLED_ShowStr(16,32,"光照:   Lx ",16); 
			OLED_ShowStr(16,48,"浊度:      ",16); 
		}
		else if(moshi==1)
		{
			OLED_ShowStr(16,24,"定时时间:      ",16);
		}
		else if(moshi==2)
		{
			OLED_ShowStr(16,24,"温度上限:      ",16);
		}
		else if(moshi==3)
		{
			OLED_ShowStr(16,24,"光照下限:      ",16);
		}
		else if(moshi==4)
		{ 
			OLED_ShowStr(16,24,"浊度上限:      ",16);
		} 
	}
	if(t1==KEY1_PRES)
	{	
		delay_ms(120);
		if(moshi==1)
		{
			if(time_t<99) time_t++;
		}
		else if(moshi==2)
		{
			if(temp_h<99) temp_h++;
		}
		else if(moshi==3)
		{
			if(ligh_l<999) ligh_l++;
		}
		else if(moshi==4)
		{ 
			if(turb_h<9999) turb_h++;
		} 
	}
	if(t1==KEY2_PRES)
	{
		delay_ms(120);
		if(moshi==1)
		{
			if(time_t>1) time_t--;
		}
		else if(moshi==2)
		{
			if(temp_h>1) temp_h--;
		}
		else if(moshi==3)
		{
			if(ligh_l>1) ligh_l--;
		}
		else if(moshi==4)
		{ 
			if(turb_h>1) turb_h--;
		} 
	}
	if(t==KEY3_PRES)
	{
		if(moshi==0)
		{
			if(js_flag==0)
			{
				js_flag = 1;
				sec = 0;min = 0; 
			}
			else  
			{
				js_flag = 0;
				sec = 0;min = 0; 
			}			
		} 
	} 
}
/*函数名：上传数据 							       */  
void Data_State(void)
{   
	u3_printf("#:%d%d:%d%d,%0.1f,%d,%d,%d#",min/10,min%10,sec/10,sec%10,(float)temperature/10,adcx1,Tur_Value,js_flag);     //temp_h,humi_h,smok_h,%d
}
int main(void)
{	 	    
	
	
	delay_init();	//延时函数初始化
	I2C_Configuration();
    OLED_Init();
	OLED_ShowStr(16,16,"Conecting...",16);  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级 
	TIM3_Int_Init(99,7199);//10Khz的计数频率10 
	usart3_init(9600);  
	esp8266_start_trans();	//esp8266进行初始化
	
	OLED_Init();			//显示初始化  
	OLED_ShowStr(16, 0,"定时:00:00 ",16);  
	OLED_ShowStr(16,16,"温度:  . ℃ ",16); 
	OLED_ShowStr(16,32,"光照:   Lx ",16); 
	OLED_ShowStr(16,48,"浊度:      ",16); 
	
	LED_Init();
	KEY_Init();  
	DS18B20_Init();		//DS18B20初始化	
	Adc_Init();			//ADC初始化	
	
	while(1)
	{	  	 
		Key_process();
		
		if(moshi==0)
		{ 
			LCD_ShowNum_0(56, 0,min,2,16);	//显示时间
			LCD_ShowNum_0(80, 0,sec,2,16);	//显示时间 	
			
			temperature=DS18B20_Get_Temp();	//获取温度
			temp_n = temperature/10;
			LCD_ShowNum(56,16,temperature/10,2,16);	//显示正数部分	    
			LCD_ShowNum(80,16,temperature%10,1,16);	//显示小数部分 
		
			adcx1 = Get_Adc_Average(ADC_Channel_1,10);
			adcx1 = (500-(adcx1/9));		//光照校准
			LCD_ShowNum(56,32,adcx1,3,16);	//显示光照
			
			adcx0 = Get_Adc_Average(ADC_Channel_0,10);	//浑浊度 
			Tur_ad = (float)adcx0/4096*3.3; 
			Tur_ad = -865.68*Tur_ad+2281.3;
			Tur_Value = (int)Tur_ad*0.65;
			LCD_ShowNum(56,48,Tur_Value,4,16);			//显示浑浊度  
			
			if(js_flag==1)	//启动定时
			{
				if(Body==1)	LEDZ = 1; else LEDZ = 0; 	//有人停止消毒
			}else LEDZ = 1;
			
			if(temp_n<temp_h)JDQ1 = 0; else JDQ1 = 1;		//加热控制
			 
			if((QS==0)&&(FY==1))JDQ2 = 0;		//启动水泵
			if((QS==1)&&(FY==0))JDQ2 = 1;		//关闭水泵
			 
			if(Tur_Value>turb_h)JDQ3 = 0; else JDQ3 = 1;	//浊度控制	 		
			if((Body==1)&&(adcx1<ligh_l))LED1 = 0; else LED1 = 1; //灯光控制
			
			if((JDQ1==0)||(JDQ2==0)||(JDQ3==0)) Buzzer = 0; else Buzzer = 1;	 //报警控制
			if(yy_flag==1)		//wifi上传数据
			{	 
				Data_State();
				yy_flag = 0;
			} 
		}
		else if(moshi==1)LCD_ShowNum(88,24,time_t,2,16); 	//显示设置值
		else if(moshi==2)LCD_ShowNum(88,24,temp_h,2,16); 
		else if(moshi==3)LCD_ShowNum(88,24,ligh_l,3,16);  
		else if(moshi==4)LCD_ShowNum(88,24,turb_h,4,16); 
		if(moshi!=0)Buzzer = 1;	
		delay_ms(20);	
	}	  
}

 
