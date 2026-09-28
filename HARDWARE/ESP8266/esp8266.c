#include "esp8266.h"
#include "string.h" 
#include "usart3.h"
#include "stm32f10x.h"
#include "sys.h" 
#include "delay.h" 

//ESP8266模块和PC进入透传模式
void esp8266_start_trans(void)
{ 
	delay_ms(1000);	
	  
	while(!CONNECT)  
	{ 
		u3_printf("AT\r\n");delay_ms(1000);
	}
	CONNECT = 0; 	
	u3_printf("AT+CWMODE=3\r\n");
	delay_ms(1000);delay_ms(1000);

	while(!CONNECT); 
	CONNECT = 0;

	u3_printf("AT+CWJAP=\"ESP8266A\",\"12345678\"\r\n");
	delay_ms(1300);delay_ms(1300);  

	while(!CONNECT);
	CONNECT	= 0;

	u3_printf("AT+CIPSTART=\"TCP\",\"tcp.tlink.io\",8647\r\n");
	delay_ms(1000);delay_ms(1000);	

	while(!CONNECT);
	CONNECT	= 0;

	u3_printf("AT+CIPMODE=1\r\n");
	delay_ms(1000);delay_ms(1000);	


	u3_printf("AT+CIPSEND\r\n");
	delay_ms(1000);delay_ms(1000);	


	u3_printf("621332087196SW4N");
	delay_ms(1000);delay_ms(1000);	
}
 




