#include "init.h"

/*
*	Flag
*/
uint8 start;
uint8 finish_flag;
uint8 MoveMode;
uint8 ReturnMode;


void flag_init(void)
{
	SkipYellow = 0;
	ReturnMode = 1;
	MoveMode = 0;
	RunMode = 0;
	finish_flag = 0;
	Threshold = 190;
	BlackThres = 130;
	PhototThreshold = 30;
	photoflag = 0;
	line_threshold = 3;
	start = 0;
	start_flag = 0;
}


void parameter_init(void)
{
	Car.Kp[0] = 0.2;
	Car.Ki[0] = 0.1;
	Car.Kd [0] = 1.0f;
	
	Car.Kp[1] = 0.0f;
	Car.Ki[1] = 0.2;
	Car.Kd [1] = 1.0f;
	Car.Yaw_Set = 0.0f;
	
	ImagePID.Kp[0] = 0.9f;
	ImagePID.Ki[0] = 0.0f;
	ImagePID.Kd[0] = 0.0f;
	
	ImagePID.Kp[1] = 0.9f;
	ImagePID.Ki[1] = 0.0f;
	ImagePID.Kd[1] = 0.0f;
	
	ImagePID.Kp[2] = 0.7f;
	ImagePID.Ki[2] = 0.0f;
	ImagePID.Kd[2] = 0.0f;
	
	ImagePID.Kp[3] = 0.7f;
	ImagePID.Ki[3] = 0.0f;
	ImagePID.Kd[3] = 0.0f;
	
	ImagePID.Kp[4] = 0.01f;
	ImagePID.Ki[4] = 0.0f;
	ImagePID.Kd[4] = 0.0f;
	
	ImagePID.Kp[5] = 0.2f;
	ImagePID.Ki[5] = 0.0f;
	ImagePID.Kd[5] = 0.0f;
	
	current_x = 1;
	current_y = -2;
	
	real_x = current_x;
	real_y = current_y;
	target_x = 1;
	target_y = -2;
	NowCntMapPoint = 0;
	CarryMapIndex[0] = 1, CarryMapIndex[1] = 3,CarryMapIndex[2] = 2;
	offset4[0][0] = -1,offset4[0][1] = 0;
	offset4[1][0] =  0,offset4[1][1] = -1;
	offset4[2][0] = 0,offset4[2][1] = 1;
	offset4[3][0] = 1,offset4[3][1] = 0;
}

void init(void)
{
	//LCD
//	lcd_init();	
	ips114_init();
	
	//Wireless USART	
//	seekfree_wireless_init();
	
	//BlueTooth
	while(bluetooth_ch9141_init()){};
	
	//Encoder
	qtimer_quad_init(QTIMER_2,QTIMER2_TIMER3_C25,QTIMER2_TIMER0_C3); 
	qtimer_quad_init(QTIMER_3,QTIMER3_TIMER2_B18,QTIMER3_TIMER3_B19);
	qtimer_quad_init(QTIMER_1,QTIMER1_TIMER3_C24,QTIMER1_TIMER2_C2);
	qtimer_quad_init(QTIMER_1,QTIMER1_TIMER0_C0,QTIMER1_TIMER1_C1);
	
	//Motor
	while(motor_init()){};
	
	//Key
	Key_Init();
	
	//Servo
	Servo_Init();
	
	//Beep
	Beep_Init();
			
	//Electromagnet
	Electromagnet_Init();
		
	//ADC
	My_ADCInit();
		
	//UART
	Uart_Init();
		
	//Camera
	mt9v03x_init();
	
	//ICM20602
	icm20602_init_spi();
		
	//Interrupt
	pit_init();  
//	pit_interrupt_ms(PIT_CH0,1);			//PID	
////	pit_interrupt_ms(PIT_CH1,20);	
//	pit_interrupt_ms(PIT_CH2,20);	
//	pit_interrupt_ms(PIT_CH3,5);	
	NVIC_SetPriority(PIT_IRQn,15);		
	
	//LED
	gpio_init(B9,GPO,0,GPIO_PIN_CONFIG); 	
	
}
