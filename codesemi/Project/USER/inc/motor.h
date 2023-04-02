#ifndef __MOTOR_H
#define __MOTOR_H
#include "zf_pwm.h"
#include "common.h"

#define motor1_dir D13
#define motor2_dir D12
#define motor3_dir D0
#define motor4_dir D1
#define PWM_1 PWM1_MODULE1_CHB_D15
#define PWM_2 PWM4_MODULE2_CHA_C30
#define PWM_3 PWM2_MODULE3_CHA_D2
#define PWM_4 PWM2_MODULE3_CHB_D3

typedef struct
{
	//parameter
	int16   last_Encoder;
	int16 	Encoder;
	float   Encoder_Integral;
	float 	error;
	float 	last_error;
	float 	last_last_error;
	float   target;
	int 	output; 
	uint8 	num;
	float   distance;   
	// PID 
	float  Kp;
	float  Ki;
	float  Kd;
}MotorDef;

int motor_init(void);
void motordef_init(void);
void motor_run(void);
void MotorStop(uint8 a,uint8 b,uint8 c,uint8 d);


#endif
