#include "headfile.h"
#include "control.h"

/*
*	Parameter
*/
CarDef Car;
ImageCorrectDef ImagePID;
float Intergral1,Intergral2,Wintergral;
uint8 PeakSpeed;
uint8 start_flag;

/*
*	Route
*/
float PathPointSUP[10][2];
float PathPointSEED[500][2];
int16 NowPathPoint;
uint8 CntPathPointSUP;
int16 CntPathPointSEED;
uint8 NowCntPathPointSUP;
uint8 NowCntMapPoint;

/*
*	Locate
*/
enum locate_step locate_state;
uint8 real_x,real_y;
float current_x,current_y;
float target_x,target_y;
float next_x,next_y;
int16 delaytime;
int baseVelocity;


/*
*	Carry
*/
uint8 SkipYellow;
uint8 RunMode;
uint8 MainClass;
uint8 CarryMapIndex[3];
float target_Cx,target_Cy;

void debug_send(int16 data[],int channel)
{
	int i=0;
	uint8 buff[4+channel*2];
	buff[0] = 0x03;								//֡ͷ
	buff[1] = 0xfc;
	for(i=0;i<channel;i++)
	{
		buff[2 + i*2] = data[i] & 0xFF;
		buff[2 + i*2 + 1] = (data[i] >> 8)&0xFF;
	}
	i = 1 + channel *2 +1;
	buff[i++] = 0xfc;								//֡β
	buff[i++] = 0x03;
	seekfree_wireless_send_buff(buff,sizeof(buff));

}

void debug_send_gyro(float data[],int channel)
{
	int i=0;
	uint8 buff[4+channel*4];
	buff[0] = 0x03;								//֡ͷ
	buff[1] = 0xfc;
	for(i=0;i<channel;i++)
	{
		buff[2 + i*4] = BYTE0(data[i]) ;
		buff[2 + i*4 + 1] = BYTE1(data[i]);
		buff[2 + i*4 + 2] = BYTE2(data[i]);
		buff[2 + i*4 + 3] = BYTE3(data[i]);
	}
	i = 1 + channel *4 +1;
	buff[i++] = 0xfc;								//֡β
	buff[i++] = 0x03;
	seekfree_wireless_send_buff(buff,sizeof(buff));

}

void Encoder_read(void)
{
	int i;
	int16 temp;
	static int16 data[4];
	for(i=0;i<4;i++)
	{		
		switch(i)
		{
			case 0:
					temp = qtimer_quad_get(QTIMER_1,QTIMER1_TIMER0_C0 );
					qtimer_quad_clear(QTIMER_1,QTIMER1_TIMER0_C0 );
					break;
			case 1:
					temp = qtimer_quad_get(QTIMER_1,QTIMER1_TIMER3_C24 );
					qtimer_quad_clear(QTIMER_1,QTIMER1_TIMER3_C24 );
					break;
			case 2:
					temp = qtimer_quad_get(QTIMER_2,QTIMER2_TIMER3_C25 );
					qtimer_quad_clear(QTIMER_2,QTIMER2_TIMER3_C25 );
					break;
			case 3:
					temp = qtimer_quad_get(QTIMER_3,QTIMER3_TIMER2_B18 );
					qtimer_quad_clear(QTIMER_3,QTIMER3_TIMER2_B18 );
					break;
		}
		Motor[i].last_Encoder = Motor[i].Encoder;
		Motor[i].Encoder = temp;
//		if(locate_state == DistanceControl)
//		{
//					Motor[i].Encoder = 1.0 * Motor[i].Encoder + 0.0 * Motor[i].last_Encoder;
//		}

		data[i] = Motor[i].Encoder;

		
		Motor[i].Encoder_Integral += 1.0 * Motor[i].Encoder + 0.0 * Motor[i].last_Encoder;
		Motor[i].distance = Motor[i].Encoder_Integral /1024*30/70*63*3.1415926; ///1024*30/68*61*3.14;//* 0.0811 + 10.079;
		
	}
//		data[0] = Motor[0].distance;
//		data[1] = Motor[1].distance;
//		data[2] = Motor[0].Encoder;
//		data[3] = Motor[1].Encoder;
//		debug_send(data,4);
		Car.distance = ( Motor[0].distance + Motor[1].distance + Motor[2].distance + Motor[3].distance)/4 * 0.985;
		Car.Xdistance = ( Motor[0].distance - Motor[1].distance + Motor[2].distance - Motor[3].distance)/4 * 0.948;

}

void motor_control(void)
{
	int i,num;
	int16 temp;
	static int16 data[8];

	for(i=0;i<4;i++)
	{
		data[i*2] = Motor[i].Encoder;
		data[i*2 +1] = Motor[i].target;
		Motor[i].last_last_error = Motor[i].last_error;
		Motor[i].last_error = Motor[i].error;
		Motor[i].error = Motor[i].target - Motor[i].Encoder;
		Motor[i].output += Motor[i].Kp*(Motor[i].error - Motor[i].last_error )+Motor[i].Ki*Motor[i].error ;//+ Motor[i].Kd*(Motor[i].last_last_error + Motor[i].error - 2*Motor[i].last_error);
	}
		
//	debug_send(data,8);
//		if(locate_state == ReturnForward && MoveMode == 1)
//		{
//			Motor[0].output = 0;
//			Motor[1].output = 0;
//		}
}

void XYWvelocity2WheelVelocity(void)
{
	if(locate_state == DistanceControl2 && RunMode == 0)
	{
		if(Car.YVelocitySet == 0.0f)
			Car.YVelocitySet = 0.0f;
		else
			Car.YVelocitySet = 0.5 * Car.YVelocitySet + 0.5 * Car.LastYVelocitySet;
		if(Car.XVelocitySet == 0.0f)
			Car.XVelocitySet = 0.0f;
		else
			Car.XVelocitySet = 0.5 * Car.XVelocitySet + 0.5 * Car.LastXVelocitySet;
		Car.LastYVelocitySet = Car.YVelocitySet;
		Car.LastXVelocitySet = Car.XVelocitySet;	
	}

	Motor[0].target = Car.YVelocitySet + Car.XVelocitySet - Car.WVelocitySet*18;
	Motor[1].target = Car.YVelocitySet - Car.XVelocitySet + Car.WVelocitySet*18;
	Motor[2].target = Car.YVelocitySet + Car.XVelocitySet + Car.WVelocitySet*18;
	Motor[3].target = Car.YVelocitySet - Car.XVelocitySet - Car.WVelocitySet*18;


//	if(locate_state == ReturnForward && MoveMode == 1)
//	{
//		Motor[0].target = 0;
//		Motor[1].target = 0;
//	}
}

void Yaw_PIDUpdate(void)
{
	//增量式
//	float temp;
//	Car.last_error = Car.error;
//	Car.error = Car.Yaw_Set - Yaw;
//	temp += Car.Kp*(Car.error - Car.last_error)  + Car.Ki * Car.error;
//	Car.WVelocitySet = -temp;
	
	//位置
	float temp;
	Car.last_error = Car.error;
	Car.error = Car.Yaw_Set - Yaw;
	temp = Car.Kp[0]*Car.error  + Car.Kd[0]*(Car.error - Car.last_error);
	Car.WVelocitySet = -temp;
	Car.WVelocitySet = ((Car.WVelocitySet) < (-3) ? (-3) : ((Car.WVelocitySet) > (3) ? (3) : (Car.WVelocitySet)));
}

void Yaw_PIDUpdate2(void)
{
	//增量式
	float temp;
	Car.last_error = Car.error;
	Car.error = Car.Yaw_Set - Yaw;
	temp += Car.Kp[1]*(Car.error - Car.last_error)  + Car.Ki[1] * Car.error;
	Car.WVelocitySet = -temp;
	Car.WVelocitySet = ((Car.WVelocitySet) < (-6) ? (-6) : ((Car.WVelocitySet) > (6) ? (6) : (Car.WVelocitySet)));
//	//位置
//	float temp;
//	Car.last_error = Car.error;
//	Car.error = Car.Yaw_Set - Yaw;
//	temp = Car.Kp*Car.error  + Car.Kd*(Car.error - Car.last_error);
//	Car.WVelocitySet = -temp;
	
}

void ImageXY_PIDUpdate(void)
{
//	float temp1;
//	static float x_last_error,x_error,x_last_last_error;
//	
//	float kp1 = 0.6,kd1 = 0.0,ki1 = 0.000;
//	x_error = 92 - Above_Centre[1];
//	x_last_error = x_error;
//	x_last_last_error = x_last_error;
//	temp1 = kp1 * x_error  + kd1 * (x_error - x_last_error) +ki1 *( x_last_last_error + x_error - 2 * x_last_error);
//	
//	Car.XVelocitySet = -temp1;
	
	ImagePID.Last_last_error[0] = ImagePID.Last_error[0];
	ImagePID.Last_error[0] = ImagePID.Error[0];
	ImagePID.Error[0] = 94 - Above_Centre[1];
	ImagePID.output[0] = ImagePID.Kp[0] * ImagePID.Error[0] + ImagePID.Kd[0] * (ImagePID.Last_last_error[0] + ImagePID.Error[0] - 2*ImagePID.Last_error[0]);
	Car.XVelocitySet = -ImagePID.output[0];
//	Car.XVelocitySet = ((Car.XVelocitySet) < (-1) ? (-1) : ((Car.XVelocitySet) > (1) ? (1) : (Car.XVelocitySet)));
	
	
	
	ImagePID.Last_last_error[1] = ImagePID.Last_error[1];
	ImagePID.Last_error[1] = ImagePID.Error[1];
	ImagePID.Error[1] = 74 - Above_Centre[0];
	ImagePID.output[1] = ImagePID.Kp[1] * ImagePID.Error[1] + ImagePID.Kd[1] * (ImagePID.Last_last_error[1] + ImagePID.Error[1] - 2*ImagePID.Last_error[1]);
	Car.YVelocitySet = ImagePID.output[1];
	
	
//	float	temp2;
//	static float y_last_error,y_error;
//	float kp2 = 0.6,kd2 = 0.0,ki2 = 0.000;
//	y_last_error = y_error;
//	y_error = 80 - Above_Centre[0];
//	//Intergral2 += y_error;
//	temp2 = kp2 * y_error + kd2 * (y_error - y_last_error);// + ki2 * Intergral2;	
//	Car.YVelocitySet = temp2;	
//	Car.YVelocitySet = ((Car.YVelocitySet) < (-15) ? (-15) : ((Car.YVelocitySet) > (15) ? (15) : (Car.YVelocitySet)));
}

void Distance_PIDUpdate(void)
{
	float temp1;
	static float last_error,error,last_last_error;
	float kp1 = 0.1,kd1 = 0.1,ki1 = 0.00;
	last_last_error = last_error;
	last_error = error;

	error = Car.distance_target[0] - Car.distance;
	
	temp1 = kp1 * error  + kd1 * (error - last_error) ;
	
	Car.YVelocitySet = temp1;
	Car.YVelocitySet = ((Car.YVelocitySet) < (-80) ? (-80) : ((Car.YVelocitySet) > (80) ? (80) : (Car.YVelocitySet)));

}

void GyroW_Control(void)
{
	//位置
	float temp;
	float kp = 0.0,ki=0.0f,kd = 0.0;
	float kp2 = 0.27f,ki2= 0.0f,kd2 = 2.5f;
	static float last_error,error,Wintergral;
	static float last_error2,error2;
	last_error = error;
	error = 0 - icm_gyro_z*0.061;
	Wintergral += error;
	Wintergral = ((Wintergral) < (-400) ? (-400) : ((Wintergral) > (400) ? (400) : (Wintergral)));
	last_error2 = error2;
	error2 = Car.Yaw_Set - Yaw;	
	
	temp = kp*error  + kd*(error - last_error) + ki * Wintergral + kp2 * error2 + kd2 * (error2 - last_error2);
	Car.WVelocitySet = -temp;
	Car.WVelocitySet = ((Car.WVelocitySet) < (-5.0) ? (-5.0) : ((Car.WVelocitySet) > (5.0) ? (5.0) : (Car.WVelocitySet)));	

}

void ImageY_PIDUpdate(void)
{
	ImagePID.Last_last_error[2] = ImagePID.Last_error[2];
	ImagePID.Last_error[2] = ImagePID.Error[2];
	ImagePID.Error[2] = 100 - Above_Centre[0];
	ImagePID.output[2] = ImagePID.Kp[2] * ImagePID.Error[2] + ImagePID.Kd[2] * (ImagePID.Last_last_error[2] + ImagePID.Error[2] - 2*ImagePID.Last_error[2]);
	Car.YVelocitySet = ImagePID.output[2];

//	ImagePID.Last_last_error[5] = ImagePID.Last_error[5];
//	ImagePID.Last_error[5] = ImagePID.Error[5];
//	ImagePID.Error[5] = PositionColumn - Above_Point[above_cnt][1];
//	ImagePID.output[5] = ImagePID.Kp[5] * ImagePID.Error[5] + ImagePID.Kd[5] * (ImagePID.Last_last_error[5] + ImagePID.Error[5] - 2*ImagePID.Last_error[5]);
//	Car.XVelocitySet = -ImagePID.output[5];
	
//	ImagePID.Last_last_error[4] = ImagePID.Last_error[4];
//	ImagePID.Last_error[4] = ImagePID.Error[4];
//	ImagePID.Error[4] = Above_Point[above_cnt][0] - Above_Point[0][0];
//	ImagePID.output[4] = ImagePID.Kp[4] * ImagePID.Error[4] + ImagePID.Kd[4] * (ImagePID.Last_last_error[4] + ImagePID.Error[4] - 2*ImagePID.Last_error[4]);
//	Car.WVelocitySet = ImagePID.output[4];	
//	Car.WVelocitySet = ((Car.WVelocitySet) < (-3) ? (-3) : ((Car.WVelocitySet) > (3) ? (3) : (Car.WVelocitySet)));	

	
}

void Yellow_PIDUpdate(void)
{
	ImagePID.Last_last_error[3] = ImagePID.Last_error[3];
	ImagePID.Last_error[3] = ImagePID.Error[3];
	ImagePID.Error[3] = 119 - Above_Centre[0];
	ImagePID.output[3] = ImagePID.Kp[3] * ImagePID.Error[3] + ImagePID.Kd[3] * (ImagePID.Last_last_error[3] + ImagePID.Error[3] - 2*ImagePID.Last_error[3]);
	Car.YVelocitySet = ImagePID.output[3];
}

//float TrapezoidalVelocityDistribution(float V0,float Vmax,float StartRate,float StopRate,float distance,float cuttentD)
//{
//	float Yspeed;
//	if(cuttentD < 0)
//		cuttentD = 0;
//	if(cuttentD < distance * StartRate)
//	{
//		Yspeed = (Vmax - V0)/(distance * StartRate) * cuttentD + V0;
//	}
//	else if(cuttentD > distance * (1 - StopRate))
//	{
//		Yspeed = Vmax/StopRate - Vmax/(StopRate * distance) * cuttentD;
//	}
//	else
//		Yspeed = Vmax;
//	
//	return Yspeed;
//}


float TrapezoidalVelocityDistribution(float V0,float Vt,float Vmax,float StartRate,float StopRate,float distance,float cuttentD)
{
	float Yspeed;
	if(cuttentD < 0)
		cuttentD = 0;
	if(cuttentD < distance * StartRate)
	{
		Yspeed = (Vmax - V0)/(distance * StartRate) * cuttentD + V0;
	}
	else if(cuttentD > distance * (1 - StopRate))
	{
//		Yspeed = Vmax/StopRate - Vmax/(StopRate * distance) * cuttentD;
		Yspeed = (Vmax - Vt) * cuttentD / ((StopRate - 1) * cuttentD) + (StopRate * Vt - Vmax)/(StopRate - 1);
	}
	else
		Yspeed = Vmax;
	
	return Yspeed;
}

float TrapezoidalVelocityDistribution2(float V0,float Vt,float k,float StartRate,float StopRate,float distance,float cuttentD)
{
	float Yspeed;
	float Vmax;
	Vmax = V0 + k * StartRate * distance;
	if(cuttentD < 0)
		cuttentD = 0;
	if(cuttentD < distance * StartRate)
	{
		Yspeed = k * cuttentD + V0;
	}
	else if(cuttentD > distance * (1 - StopRate))
	{
//		Yspeed = Vmax/StopRate - Vmax/(StopRate * distance) * cuttentD;
		Yspeed = -k * cuttentD  + Vmax + (1 - StopRate)*distance*k;
	}
	else
		Yspeed = Vmax;
	
	return Yspeed;
}



float SVelocityDistribution2(float V0,float Vt,float k,float StartRate,float StopRate,float distance,float cuttentD)
{
	float Yspeed;
	float Vmax;
	Vmax = V0 + k * StartRate * distance;
	if(cuttentD < 0)
		cuttentD = 0;
	if(cuttentD < distance * StartRate)
	{
		Yspeed = k * cuttentD + V0;
	}
	else if(cuttentD > distance * (1 - StopRate))
	{
//		Yspeed = Vmax/StopRate - Vmax/(StopRate * distance) * cuttentD;
		Yspeed = -k * cuttentD  + Vmax + (1 - StopRate)*distance*k;
	}
	else
		Yspeed = Vmax;
	
	return Yspeed;
}


float TrapezoidalVelocityDistribution3(float V0,float Vt,float k,float StartRate,float StopRate,float distance,float cuttentD)
{
	float Yspeed;
	float Vmax;
	float k1;
	Vmax = V0 + k * StartRate * distance;
	k1 = (Vmax - Vt)/(distance * StopRate);
	if(cuttentD < 0)
		cuttentD = 0;
	else  if(cuttentD >= distance)
	{
		Yspeed = 0;
		return Yspeed;
	}
	if(cuttentD < distance * StartRate)
	{
		Yspeed = k * cuttentD + V0;
	}
	else if(cuttentD > distance * (1 - StopRate))
	{
//		Yspeed = Vmax/StopRate - Vmax/(StopRate * distance) * cuttentD;
		Yspeed = -k1 * cuttentD  + Vmax + (1 - StopRate)*distance*k1;
//		Yspeed = 0;
	}
	else
		Yspeed = Vmax;
	
	return Yspeed;
}

float TrapezoidalVelocityDistribution5(float V0,float Vt,float peakspeed,float StartRate,float stopdistance,float distance,float cuttentD)
{
	float Yspeed;
	float Vmax;
	float k1;
	float k;
	float StopRate;
	StopRate = stopdistance /  distance;
	Vmax = peakspeed;
	if(StopRate != 0)
	{
			k1 = (Vmax - Vt)/(distance * StopRate);
	
	}
	else
			k1 = 0;

	k = (Vmax - V0)/(distance * StartRate);
	if(cuttentD < 0)
		cuttentD = 0;
	else  if(cuttentD >= distance)
	{
		Yspeed = 0;
		return Yspeed;
	}
	if(cuttentD < distance * StartRate)
	{
		Yspeed = k * cuttentD + V0;
	}
	else if(cuttentD > distance * (1 - StopRate))
	{
//		Yspeed = Vmax/StopRate - Vmax/(StopRate * distance) * cuttentD;
		Yspeed = -k1 * cuttentD  + Vmax + (1 - StopRate)*distance*k1;
//		Yspeed = 0;
	}
	else
		Yspeed = Vmax;
	
	return Yspeed;
}

void Correct_velocity(void)
{
	float ratioX,ratioY;	
	float tmp;
	float rate1,rate2;
	float k;
	float stopdistance,peakspeed,stopV;
	float time;
	Car.distance_target[0] = (PathPointSUP[NowCntPathPointSUP][0] - current_x) * 200;
	Car.distance_target[1] = (PathPointSUP[NowCntPathPointSUP][1] - current_y) * 200;
	Car.distance_target[2] = Car.distance_target[0] - Car.Xdistance;
	Car.distance_target[3] = Car.distance_target[1] - Car.distance;	
	Car.distance_target[4] = (PathPointSUP[NowCntPathPointSUP][0] - PathPointSUP[NowCntPathPointSUP-1][0]) * 200;
	Car.distance_target[5] = (PathPointSUP[NowCntPathPointSUP][1] - PathPointSUP[NowCntPathPointSUP-1][1]) * 200;
	ratioX = Car.distance_target[2];
	ratioY = Car.distance_target[3];
	tmp = sqrt(pow(ratioX,2) + pow(ratioY,2));
	if(abs(ratioX) < 10)
		ratioX = 0;
	else
		ratioX = ratioX / tmp;
	if(abs(ratioY) < 10)	
		ratioY = 0;
	else
		ratioY = ratioY / tmp;
	if(Car.distance_target[5] > 0)
	{
		rate1 = 0.1;
		stopV = 0;
		if(sqrt(pow(Car.distance_target[4],2) + pow(Car.distance_target[5],2)) > 4000)
		{
			peakspeed = 120;
			stopdistance = 0;
		}
		else if(sqrt(pow(Car.distance_target[4],2) + pow(Car.distance_target[5],2)) > 2000)
		{
			peakspeed = 120;
			stopdistance = 200;	
		}
		else if(sqrt(pow(Car.distance_target[4],2) + pow(Car.distance_target[5],2)) > 1000)
		{
			peakspeed = 90;
			stopdistance = 120;	
		}
		else
		{
			peakspeed = 50;
			stopdistance = 0;
		}	
	}
	else
	{
			peakspeed = 90;
			stopdistance = 0;
			rate1 = 0.2;
			stopV = 50;
	}


//	baseVelocity = TrapezoidalVelocityDistribution(10,PeakSpeed,0.1,0.1,sqrt(pow(Car.distance_target[0],2) + pow(Car.distance_target[1],2)),sqrt(pow(Car.Xdistance,2) + pow(Car.distance,2)));
//	baseVelocity = TrapezoidalVelocityDistribution(30,50,PeakSpeed,0.1,0.1,sqrt(pow(Car.distance_target[0],2) + pow(Car.distance_target[1],2)),sqrt(pow(Car.Xdistance,2) + pow(Car.distance,2)));
//	baseVelocity = TrapezoidalVelocityDistribution2(PeakSpeed,PeakSpeed,rate,time,time,sqrt(pow(Car.distance_target[0],2) + pow(Car.distance_target[1],2)),sqrt(pow(Car.Xdistance,2) + pow(Car.distance,2)));
//	baseVelocity = TrapezoidalVelocityDistribution3(30,30,k,rate1,rate2,sqrt(pow(Car.distance_target[0],2) + pow(Car.distance_target[1],2)),sqrt(pow(Car.Xdistance,2) + pow(Car.distance,2)));
		if(sqrt(pow(Car.distance_target[4],2) + pow(Car.distance_target[5],2)) < 625)
		{
			baseVelocity = 50;
		
		}
		else
			baseVelocity = TrapezoidalVelocityDistribution5(50,stopV,peakspeed,rate1,stopdistance,sqrt(pow(Car.distance_target[0],2) + pow(Car.distance_target[1],2)),sqrt(pow(Car.Xdistance,2) + pow(Car.distance,2)));
		Car.XVelocitySet = ratioX * baseVelocity;
		Car.YVelocitySet = ratioY * baseVelocity;		





}

void Correct_velocity2(uint8 flag)
{
	static float delt_x,delt_y;
	float offset_X,offset_Y;
	float tmpX,tmpY;
	float ratioX,ratioY;
	static float theta;
	static float targetDX,targetDY;
	static float targetX,targetY;
	static uint8 TargetNum;
	static uint8 CurrentNum;
	if(flag == 1)			//first enter function
	{
		offset_X = (target_x - current_x) * 200;
		offset_Y = (target_y - current_y) * 200;
		if(abs(Yaw) < 5)	//0°
		{
			delt_x = offset_X;
			delt_y = offset_Y;
		}
		else if(abs(Yaw-90) < 5) // 90°
		{
			delt_x = -offset_Y;
			delt_y = offset_X;
		}
		else if(abs(Yaw+90 < 5) || abs(Yaw-270) < 5)	//-90 || 270
		{
			delt_x = offset_Y;
			delt_y = -offset_X;
		}
		else if(abs(Yaw+180)<5 || abs(Yaw-180) <5)	//180 || -180
		{
			delt_x = -offset_X;
			delt_y = -offset_Y;
		}	
		TargetNum = ( pow(delt_x,2) + pow(delt_y,2) ) / 100;
		CurrentNum = 1;
		theta = acos((delt_x/sqrt(pow(delt_x,2) + pow(delt_y,2))))*180/3.1415926;
		if(delt_y < 0)
			theta = 360 - theta;
		tmpX = 0.0f;
		tmpY = 0.0f;
		targetX = CurrentNum*100*cos(theta/180*3.1415926);
		targetY = CurrentNum*100*sin(theta/180*3.1415926);
		targetDX = targetX - tmpX;
		targetDY = targetY - tmpY;
	}
	
	tmpX = (Motor[0].distance - Motor[1].distance)*0.5;
	tmpY = (Motor[0].distance + Motor[1].distance)*0.5;	
	if(CurrentNum <= TargetNum+1)
	{
		if(abs(targetDX) > abs(targetDY))
		{
			if(abs(tmpX - targetX) < 10 )
			{
				CurrentNum++;		
				if(CurrentNum == TargetNum+1)
				{
					targetX = delt_x;
					targetY = delt_y;
				}
				else if(CurrentNum <= TargetNum)
				{
					targetX = CurrentNum*100*cos(theta/180*3.1415926);
					targetY = CurrentNum*100*sin(theta/180*3.1415926);	
					targetDX = targetX - tmpX;
					targetDY = targetY - tmpY;
				}
				
			}	
		}
		else
		{
			if(abs(tmpY - targetY)< 10)
			{
				CurrentNum++;		
				if(CurrentNum == TargetNum+1)
				{
					targetX = delt_x;
					targetY = delt_y;
				}
				else if(CurrentNum <= TargetNum)
				{
					targetX = CurrentNum*100*cos(theta/180*3.1415926);
					targetY = CurrentNum*100*sin(theta/180*3.1415926);	
					targetDX = targetX - tmpX;
					targetDY = targetY - tmpY;
				}
				
			}			
		
		}
	}
	if(CurrentNum > TargetNum+1)
	{
		Car.XVelocitySet = 0;
		Car.YVelocitySet = 0;	
		return;	
	}
	
	ratioX = targetX - tmpX;
	ratioY = targetY - tmpY;
	if(abs(ratioX) < 10)
		ratioX = 0;
	else
		ratioX = ratioX / sqrt(pow(ratioX,2) + pow(ratioY,2));
	if(abs(ratioY) < 10)	
		ratioY = 0;
	else
		ratioY = ratioY / sqrt(pow(ratioX,2) + pow(ratioY,2));
	Car.XVelocitySet = ratioX * baseVelocity;
	Car.YVelocitySet = ratioY * baseVelocity;
	




}

/*
*	5ms Update Velocity
*/
void Correct_velocity3(void)
{
	int16 data1[4];
	float ratioX,ratioY;
	float tmpX,tmpY;
	tmpX = (Motor[0].distance - Motor[1].distance + Motor[2].distance - Motor[3].distance)/4;
	tmpY = (Motor[0].distance + Motor[1].distance + Motor[2].distance + Motor[3].distance)/4;
	ratioX = PathPointSEED[NowPathPoint][0] - tmpX;
	ratioY = PathPointSEED[NowPathPoint][1] - tmpY;
	data1[0] = (int16)tmpX;
	data1[1] = (int16)PathPointSEED[NowPathPoint][0];
	data1[2] = (int16)tmpY;
	data1[3] = (int16)PathPointSEED[NowPathPoint][1];
	debug_send(data1,4);
	if(abs(ratioX) < 10)
		ratioX = 0;
	else
		ratioX = ratioX / sqrt(pow(ratioX,2) + pow(ratioY,2));
	if(abs(ratioY) < 10)	
		ratioY = 0;
	else
		ratioY = ratioY / sqrt(pow(ratioX,2) + pow(ratioY,2));
	Car.XVelocitySet = ratioX * baseVelocity;
	Car.YVelocitySet = ratioY * baseVelocity;
	

}


/*
*	Update Velocity per point
*/
void Correct_velocity4(void)
{
	int16 data1[4];
	float ratioX,ratioY;	
	float tmp;
	Car.distance_target[0] = PathPointSEED[NowPathPoint][0];
	Car.distance_target[1] = PathPointSEED[NowPathPoint][1];
	Car.distance_target[2] = PathPointSEED[NowPathPoint][0] - Car.Xdistance;
	Car.distance_target[3] = PathPointSEED[NowPathPoint][1] - Car.distance;
	ratioX = Car.distance_target[2];
	ratioY = Car.distance_target[3];
	tmp = sqrt(pow(ratioX,2) + pow(ratioY,2));
	if(abs(ratioX) < 10)
		ratioX = 0;
	else
		ratioX = ratioX / tmp;
	if(abs(ratioY) < 10)	
		ratioY = 0;
	else
		ratioY = ratioY / tmp;
	if(RunMode == 0)
	{
//		baseVelocity = TrapezoidalVelocityDistribution(10,100,0.1,0.1,sqrt(pow(PathPointSEED[CntPathPointSEED-1][0],2) + pow(PathPointSEED[CntPathPointSEED-1][1],2)),sqrt(pow(Car.Xdistance,2) + pow(Car.distance,2)));	
	}
//	else if(RunMode == 1)
//	{
//		baseVelocity = TrapezoidalVelocityDistribution(80,80,0,0,sqrt(pow(PathPointSEED[CntPathPointSEED-1][0],2) + pow(PathPointSEED[CntPathPointSEED-1][1],2)),sqrt(pow(Car.Xdistance,2) + pow(Car.distance,2)));	
//	}
	Car.XVelocitySet = ratioX * baseVelocity;
	Car.YVelocitySet = ratioY * baseVelocity;
	data1[0] = Car.XVelocitySet;
	data1[1] = Car.YVelocitySet;
	data1[2] = Car.WVelocitySet;
	debug_send(data1,3);
}


void Distance_Clear(void)
{
	Motor[0].Encoder_Integral = 0;
	Motor[1].Encoder_Integral = 0;
	Motor[2].Encoder_Integral = 0;
	Motor[3].Encoder_Integral = 0;
	Motor[0].distance = 0;
	Motor[1].distance = 0;
	Motor[2].distance = 0;
	Motor[3].distance = 0;
	Car.distance = 0;
	Car.Xdistance = 0;
}

void PIDError_Clear(void)
{
	ImagePID.Last_last_error[0] = 0.0f,ImagePID.Last_error[0] = 0.0f,ImagePID.Error[0] = 0.0f;
	ImagePID.Last_last_error[1] = 0.0f,ImagePID.Last_error[1] = 0.0f,ImagePID.Error[1] = 0.0f;
}