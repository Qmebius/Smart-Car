#include "servo.h"


void Servo_Init(void)
{
	pwm_init(S_MOTOR1_PIN,50,2.4*50000/20);			//1.45
}

void Servo_SetDegree(uint8 degree)
{
	uint8 duty = (degree/180 * 2 + 0.5)/20 * 50000; 
	pwm_duty(S_MOTOR1_PIN,duty);

}