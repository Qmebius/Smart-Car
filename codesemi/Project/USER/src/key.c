#include "key.h"

uint8 KEY_Top_status = 1;
uint8 KEY_Bottom_status = 1;
uint8 KEY_Left_status = 1;
uint8 KEY_Right_status = 1;
uint8 KEY_Middle_status = 1;

uint8 KEY_Top_Last_status;
uint8 KEY_Bottom_Last_status;
uint8 KEY_Left_Last_status;
uint8 KEY_Right_Last_status;
uint8 KEY_Middle_Last_status;

uint8 KEY_Top_flag;
uint8 KEY_Bottom_flag;
uint8 KEY_Left_flag;
uint8 KEY_Right_flag;
uint8 KEY_Middle_flag;

void Key_Init(void)
{
	gpio_init(KEY_Top,GPI,0,GPIO_PIN_CONFIG);
	gpio_init(KEY_Bottom,GPI,0,GPIO_PIN_CONFIG);
	gpio_init(KEY_Left,GPI,0,GPIO_PIN_CONFIG);
	gpio_init(KEY_Right,GPI,0,GPIO_PIN_CONFIG);
	gpio_init(KEY_Middle,GPI,0,GPIO_PIN_CONFIG);
	
	KEY_Top_status = 1;
	KEY_Bottom_status = 1;
	KEY_Left_status = 1;
	KEY_Right_status = 1;
	KEY_Middle_status = 1;
}


void Key_Scan(void)
{
	KEY_Top_Last_status = KEY_Top_status;
	KEY_Top_status = gpio_get(KEY_Top);
	if(KEY_Top_status && !KEY_Top_Last_status)    KEY_Top_flag = 1;

	KEY_Bottom_Last_status = KEY_Bottom_status;
	KEY_Bottom_status = gpio_get(KEY_Bottom);
	if(KEY_Bottom_status && !KEY_Bottom_Last_status)    KEY_Bottom_flag = 1;

	KEY_Left_Last_status = KEY_Left_status;
	KEY_Left_status = gpio_get(KEY_Left);
	if(KEY_Left_status && !KEY_Left_Last_status)    KEY_Left_flag = 1;

	KEY_Right_Last_status = KEY_Right_status;
	KEY_Right_status = gpio_get(KEY_Right);
	if(KEY_Right_status && !KEY_Right_Last_status)    KEY_Right_flag = 1;

	KEY_Middle_Last_status = KEY_Middle_status;
	KEY_Middle_status = gpio_get(KEY_Middle);
	if(KEY_Middle_status && !KEY_Middle_Last_status)    KEY_Middle_flag = 1;

	if(KEY_Top_flag)
	{

		KEY_Top_flag = 0;
	}
	
	if(KEY_Bottom_flag)
	{

		KEY_Bottom_flag = 0;
	}
	
	if(KEY_Left_flag)
	{
		BlackThres++;	
		KEY_Left_flag = 0;
	}
	
	if(KEY_Right_flag)
	{
		BlackThres--;	
		KEY_Right_flag = 0;
	}
	
	if(KEY_Middle_flag)
	{
		start++;
		KEY_Middle_flag = 0;
	}
	



}
