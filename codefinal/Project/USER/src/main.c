/***************************************************************
 * @author     		mxq
 * @Target core		NXP RT1064DVL6A
 * @date       		2021-11-17
****************************************************************/

#include "headfile.h"

int main(void)
{
	DisableGlobalIRQ();
    board_init();
	systick_delay_ms(300);
	
	parameter_init();
	flag_init();
	init();
	
    EnableGlobalIRQ(0);	
	systick_start();
	int16 data1[6];
    while (1)
    {	
		Key_Scan();
		if(start ==0)
		{	
			Yaw = 0;
			target_x = 26,target_y = 2.5;
			circlecount = 2;
			NowCntMapPoint = circlecount;
			coordinate2DIS_DEGE();
			if(MoveMode == 2)
			{
				locate_state = DistanceControl2;
//				Correct_velocity3();
				Distance_Clear();
//				Correct_velocity4();
				Correct_velocity();
				
			}			
			pit_interrupt_ms(PIT_CH1,1);	
//			pit_interrupt_ms(PIT_CH2,5);	

			ips114_clear(WHITE);
			start = 4;			
			
		}

		if(start == 4)
		{
//			data1[0] = (int16)Car.Xdistance;
//			data1[1] = (int16)Car.distance_target[0];
//			data1[2] = (int16)Car.distance;
//			data1[3] = (int16)Car.distance_target[1];
//			data1[4] = NowPathPoint;
//			data1[5] = CntPathPointSEED;
//			debug_send(data1,6);

			ips114_showfloat(20,0,current_x,5,2),ips114_showfloat(60,0,current_y,5,2);
			ips114_showfloat(20,1,target_x,5,2),ips114_showfloat(60,1,target_y,5,2);
			ips114_showfloat(20,3,PathPointSUP[NowCntPathPointSUP][0],5,2),ips114_showfloat(80,3,PathPointSUP[NowCntPathPointSUP][1],5,2);
			ips114_showuint8(20,2,NowCntPathPointSUP),ips114_showuint8(60,2,NowCntMapPoint);
			ips114_showfloat(200,1,Yaw,5,2);
//			ips114_showuint8(200,4,Above_Point[above_cnt][1]);
			ips114_showuint8(200,0,locate_state);
//			ips114_displayimage032_zoom(Bin_Image[0], MT9V03X_W, MT9V03X_H, MT9V03X_W, MT9V03X_H);			
//			ips114_showfloat(200,4,Car.Yaw_Set,5,2);
			ips114_showfloat(200,2,Car.YVelocitySet,5,2);
			ips114_showfloat(200,3,Car.XVelocitySet,5,2);
			ips114_showint16(180,5,icm_gyro_z);
//			ips114_showuint8(200,4,NowPathPoint);
//			ips114_showuint8(200,5,CntPathPointSEED);
//			ips114_showfloat(200,6,PathPointSEED[CntPathPointSEED-1][0],5,2);
//			ips114_showfloat(200,7,PathPointSEED[CntPathPointSEED-1][1],5,2);
			ips114_showfloat(200,6,Car.distance_target[0],5,2);
			ips114_showfloat(200,7,Car.distance_target[1],5,2);
			ips114_showfloat(20,6,Car.Xdistance,5,2);
			ips114_showfloat(20,7,Car.distance,5,2);
//			ips114_showuint8(200,2,Above_Centre[0]),ips114_showuint8(200,3,Above_Centre[1]);

		}


	}

}
