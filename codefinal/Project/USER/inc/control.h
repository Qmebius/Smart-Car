#ifndef __CONTROL_H
#define __CONTROL_H
#include "headfile.h"

typedef struct
{
	float XVelocitySet;
	float YVelocitySet;
	float WVelocitySet;
	float LastXVelocitySet;
	float LastYVelocitySet;
	float LastWVelocitySet;
	float distance;
	float Xdistance;
	float distance_shadow;
	float distance_target[6];
	float Yaw_Set;
	float Yaw_SetShadow;
	float Kp[2];
	float Ki[2];
	float Kd[2];
	float error;
	float last_error;
	float last_last_error;
}CarDef;

typedef struct
{
	float Kp[6];
	float Ki[6];
	float Kd[6];
	float Last_last_error[6];
	float Last_error[6];
	float Error[6];
	float output[6];
}ImageCorrectDef;

enum locate_step{
				 DegreeControl,DegreeDelay,
				 DistanceControl,DistanceControl2,DistanceDelay,
//				 imageYcorrect,
//				 ReturnForward,ReturnDelay,
				 ImageCorrect,ImageCorrectDelay,
				 UartStart,UartWait,
				 PutOn1,PutOnDelay1,PutOn2,PutOnDelay2,
				 CarryUpdate,
//				 CarryReturnForward,
//				 YellowCorrect,
				 PutDown1,PutDownDelay1,PutDown2,PutDownDelay2,
				 UpdateTarget,FinishState
				};
	
#define BYTE0(dwTemp)       ( *( (char *)(&dwTemp)    ) )
#define BYTE1(dwTemp)       ( *( (char *)(&dwTemp) + 1) )
#define BYTE2(dwTemp)       ( *( (char *)(&dwTemp) + 2) )
#define BYTE3(dwTemp)       ( *( (char *)(&dwTemp) + 3) )

/*
*	Communication
*/
void debug_send(int16 data[],int channel);
void debug_send_gyro(float data[],int channel);

/*
*	Motor Control
*/
void Encoder_read(void);
void motor_control(void);

/*
*	Angle Control
*/
void Yaw_PIDUpdate(void);
void Yaw_PIDUpdate2(void);
void GyroW_Control(void);


/*
*	Speed Update
*/
void XYWvelocity2WheelVelocity(void);

/*
*	Locate
*/


/*
*	Image Control
*/
void ImageY_PIDUpdate(void);
void ImageXY_PIDUpdate(void);
void Yellow_PIDUpdate(void);

/*
*	Distance Control
*/
void Distance_PIDUpdate(void);


//float TrapezoidalVelocityDistribution(float V0,float Vmax,float StartRate,float StopRate,float distance,float cuttentD);
float TrapezoidalVelocityDistribution(float V0,float Vt,float Vmax,float StartRate,float StopRate,float distance,float cuttentD);
float TrapezoidalVelocityDistribution2(float V0,float Vt,float k,float StartRate,float StopRate,float distance,float cuttentD);
float TrapezoidalVelocityDistribution3(float V0,float Vt,float k,float StartRate,float StopRate,float distance,float cuttentD);
float TrapezoidalVelocityDistribution5(float V0,float Vt,float peakspeed,float StartRate,float stopdistance,float distance,float cuttentD);

/*
*	Omni Move
*/
void Correct_velocity(void);
void Correct_velocity2(uint8 flag);
void Correct_velocity3(void);
void Correct_velocity4(void);


/*
*	Clear
*/
void Distance_Clear(void);
void PIDError_Clear(void);
#endif