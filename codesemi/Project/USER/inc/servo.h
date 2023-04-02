#ifndef __SERVO__H
#define __SERVO__H

#include "headfile.h"
#define S_MOTOR1_PIN PWM4_MODULE3_CHA_C31

void Servo_Init(void);
void Servo_SetDegree(uint8 degree);

#endif