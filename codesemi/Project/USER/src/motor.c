#include "motor.h"
#include "headfile.h"

MotorDef Motor[4];
uint8 motor_count;


void motordef_init(void)
{
	int i;
	for(i=0;i<4;i++)
	{
		Motor[i].last_Encoder = 0;
		Motor[i].Encoder = 0;
		Motor[i].error = 0.0f;
		Motor[i].last_error = 0.0f;
		Motor[i].last_last_error = 0.0f;
		Motor[i].num = i+1;
		Motor[i].output = 0;
		Motor[i].target = 0;
		
		Motor[i].Kp = 270.0f;
		Motor[i].Ki = 55.0f;
		Motor[i].Kd = 200.0f;
	}
//		Motor[0].Kp = 340.0f;
//		Motor[0].Ki = 74.0f;
//		Motor[0].Kd = 250.0f;
//	
//		Motor[1].Kp = 280.0f;
//		Motor[1].Ki = 70.0f;
//		Motor[1].Kd = 250.0f;
//	
//		Motor[2].Kp = 340.0f;
//		Motor[2].Ki = 65.0f;
//		Motor[2].Kd = 250.0f;
//	
//		Motor[3].Kp = 289.0f;
//		Motor[3].Ki = 58.0f;
//		Motor[3].Kd = 250.0f;
}

int motor_init(void)
{
	motordef_init();
	gpio_init(motor1_dir, GPO, 0, GPIO_PIN_CONFIG); 		
	gpio_init(motor2_dir, GPO, 0, GPIO_PIN_CONFIG); 	
	gpio_init(motor3_dir, GPO, 0, GPIO_PIN_CONFIG); 	
	gpio_init(motor4_dir, GPO, 0, GPIO_PIN_CONFIG); 			
	pwm_init(PWM_1,17*1000,0);
	pwm_init(PWM_2,17*1000,0);
	pwm_init(PWM_3,17*1000,0);
	pwm_init(PWM_4,17*1000,0);

	return 0;
}


void motor_run(void)
{
	int i,DIR,duty;
	for(i=0;i<4;i++)
	{
		duty = Motor[i].output;
		if(locate_state == FinishState)
			duty = 0;
		if(duty > 0)
				DIR = 1;
		else
		{
				DIR = 0;
				duty = -duty;
		}
		duty = ( (duty) < (0)  ? (0) : ( (duty) > (30000) ? (30000) : (duty) ) );
		switch(i)
		{
			case 0:
				gpio_set(motor1_dir,DIR);
				pwm_duty(PWM_1,duty);
				break;
			case 1:
				gpio_set(motor2_dir,DIR);
				pwm_duty(PWM_2,duty);			
				break;
			case 2:
				gpio_set(motor3_dir,DIR);
				pwm_duty(PWM_3,duty);
				break;	
			case 3:
				gpio_set(motor4_dir,DIR);
				pwm_duty(PWM_4,duty);
				break;				
		}
	}
}

void MotorStop(uint8 a,uint8 b,uint8 c,uint8 d)
{
	if(a)
		Motor[0].output = 0;
	if(b)
		Motor[1].output = 0;
	if(c)
		Motor[2].output = 0;
	if(d)
		Motor[3].output = 0;
}






