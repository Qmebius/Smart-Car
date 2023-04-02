#include "my_math.h"
#include "headfile.h"

void coordinate2DIS_DEGE(void)
{
//	static int i=0;
	float antirotate,rotate;
	float distance;
	float delt_x,delt_y;
//	real_x = map[NowCntMapPoint][0];
//	real_y = map[NowCntMapPoint][1];
	if(RunMode == 0)
	{
		current_x = target_x;
		current_y = target_y;
	}
	else if(RunMode == 1)
	{
		current_x = target_Cx;
		current_y = target_Cy;		
	}	
	RunMode = 0;
	if(NowCntMapPoint == circlecount)
	{
		finish_flag = 1;
		
	}
	else if(NowCntMapPoint == circlecount + 1)
	{
		finish_flag = 2;
		return;		
	}
	if(NowCntMapPoint < circlecount && NowCntMapPoint >0)
	{
		target_x = next_x;
		target_y = next_y;
//		target_x = map[NowCntMapPoint+1][0];
//		target_y = map[NowCntMapPoint+1][1] - 1;	
	}
	else if(NowCntMapPoint == 0)
	{
		target_x = next_x;
		target_y = next_y;
		start_flag = 1;
	}
//	else if(NowCntMapPoint == circlecount)
//	{
//		target_x = map[NowCntMapPoint+1][0];
//		target_y = map[NowCntMapPoint+1][1];	
//	}
	
//	distance = sqrt(pow((target_x -  current_x),2) + pow((current_y - target_y),2))*200;
//	antirotate = fun_A(target_x - current_x)*acos(((target_y - current_y)/sqrt(pow((target_x - current_x),2) + pow((target_y - current_y),2) )))*180/3.1415926;
//	rotate = (antirotate) < (0) ? (360 + antirotate) :antirotate;
	NowCntMapPoint++;
//	if(distance <= 100)
		MoveMode = 2;
//	else
//		MoveMode = 1;
	
	//run forward
	if(MoveMode == 1)
	{
		Car.distance_target[0] = distance ;
		if(abs(Yaw) < 10)
		{
			Car.Yaw_Set = antirotate;
			return;
		}
		if(abs(Yaw - 180) < 10)
		{
			if(antirotate < 0)
				Yaw -=360;
			Car.Yaw_Set = antirotate;
			return;
		}		
		if( antirotate >= -90 && antirotate <= 90)
			Car.Yaw_Set = antirotate;
		else
		{
			Car.Yaw_Set = rotate;	
			Yaw = Yaw < 0 ? (Yaw+360) : Yaw;
		}
//		ips114_showfloat(20,4,Car.Yaw_Set,5,2);		
	}
	
	//translation
	if(MoveMode == 2)
	{
		PathPointSUP_Programme();
//		PathPointSEED_Programme();
		NowPathPoint = 0;
		NowCntPathPointSUP = 1;
		if(abs(Yaw) <= 10) //0¡ã
		{
			Car.Yaw_Set = 0.0f;
//			delt_x = target_x -  current_x;
//			delt_y = target_y -  current_y; 
//			Car.distance_target[0] = PathPointSEED[NowPathPoint][0] - PathPointSEED[NowPathPoint-1][0];
//			Car.distance_target[1] = PathPointSEED[NowPathPoint][1] - PathPointSEED[NowPathPoint-1][1];
//			delt_x = delt_x / sqrt(pow(delt_x,2) + pow(delt_y,2));
//			delt_y = delt_y / sqrt(pow(delt_x,2) + pow(delt_y,2));
//			Car.XVelocitySet = delt_x * baseVelocity;
//			Car.YVelocitySet = delt_y * baseVelocity;	
		}


	
	}

}

void CarryDIS_DEG(void)
{
	float antirotate,rotate;
	float distance;
	float delt_x,delt_y;
	float tmp;
	current_x = target_x;
	current_y = target_y;
	
	switch(CarryMapIndex[MainClass - 1])
	{
		//1 £ºleft  2 : top  3 : right
		case 1:
			target_Cx = -0.75;
			target_Cy = current_y;
			antirotate = -90.0f;
			break;
		case 2:
			target_Cx = current_x;
			target_Cy = 21;
			antirotate = 0.0f;
			break;
		case 3:
			target_Cx = 26.75;
			target_Cy = current_y;	
			antirotate = 90.0f;
			break;
	}
	Find_BestPoint(target_Cx,target_Cy);
	switch(CarryMapIndex[MainClass - 1])
	{
		//1 £ºleft  2 : top  3 : right
		case 1:
			target_Cy = (current_y + next_y)/2;
			break;
		case 2:
			target_Cx = (current_x + next_x)/2;
			break;
		case 3:
			target_Cy = (current_y + next_y)/2;	
			break;
	}	
//	distance = sqrt(pow((target_Cx -  current_x),2) + pow((target_Cy - current_y),2))*200 -200;
//	if(distance < 0)
//	{
//		SkipYellow = 1;	
//		distance = 0;
//	}

	

//	if(distance <= 100)
		MoveMode = 2;
//	else
//		MoveMode = 1;	
	//run forward
//	if(MoveMode == 1)
//	{
//		Car.distance_target[0] = distance;
//		rotate = (antirotate) < (0) ? (360 + antirotate) :antirotate;
//		
//		if(abs(Yaw - 0) <= 10) //0¡ã
//			Car.Yaw_Set = antirotate;
//		else if(abs(Yaw - 90) <= 10) //90¡ã
//			Car.Yaw_Set = antirotate;
//		else if(abs(Yaw - 180) <= 10) //180¡ã
//		{
//			if(antirotate < 0)
//				Yaw  -= 360;
//			Car.Yaw_Set = antirotate;		
//		}
//		else										   //-90¡ã
//			Car.Yaw_Set = antirotate;
//	}	
	if(MoveMode == 2)
	{
		if(abs(Yaw) <= 10) //0¡ã
		{
			Car.Yaw_Set = 0.0f;
//			delt_x = target_Cx -  current_x;
//			delt_y = target_Cy -  current_y; 
//			Car.distance_target[0] = delt_y * 200;
//			Car.distance_target[1] = delt_x * 200;
//			tmp = sqrt(pow(delt_x,2) + pow(delt_y,2));
//			delt_x = delt_x / tmp;
//			delt_y = delt_y / tmp;
//			Car.XVelocitySet = delt_x * 80;
//			Car.YVelocitySet = delt_y * 80;	
		}
		PathPointSUP_Programme();
		NowPathPoint = 0;
		NowCntPathPointSUP = 1;
//		PathPointSEED_Programme();
//		NowPathPoint = 0;
	
	}
	
	
}

int fun_A(int data)
{
	if(data >= 0)
		return 1;
	else
		return -1;
	
}

float my_rand(int16 accuracy)
{
	float data;
	int seed = systick_getval_ns(); 
	accuracy = accuracy * 10 -1;
	srand(seed);//Initialize Rand Seed
	data = rand() % ( accuracy + 1) /(float)(accuracy+1);
	return data;
}


int Rand_Integer(int min,int max)
{
	int data;
	int seed = systick_getval_ns(); 
	srand(seed);
	data = rand() % (max - min +1) + min;
	return data;
}

int FalgFunction(int data)
{
	if(data > 0)
		return 1;
	else if(data < 0)
		return -1;
	else
		return 0;
	
}

void PathPointSUP_Programme(void)
{
	uint8 i=0,num;
	float x,y;
	CntPathPointSUP = 0;
	PathPointSUP[i][0] = current_x;
	PathPointSUP[i][1] = current_y;
	i++;
	if(start_flag == 1)
	{
		start_flag = 0;
		PathPointSUP[i][0] = 1;
		PathPointSUP[i][1] = 2 ;		
		i++;
		PathPointSUP[i][0] = next_x;
		PathPointSUP[i][1] = next_y;
	}
	else if(finish_flag == 1)
	{
		PathPointSUP[i][0] = 1.5;
		PathPointSUP[i][1] = 2;
		i++;
		PathPointSUP[i][0] = 1.5;
		PathPointSUP[i][1] = -1.5;		
	
	}
	else
	{
		if(RunMode == 0)
		{
			if(target_y < current_y)
			{
				if(target_x > current_x)
				{
					PathPointSUP[i][0] = target_x - 1;
					PathPointSUP[i][1] = target_y ;
				}
				else
				{
					PathPointSUP[i][0] = target_x + 1;
					PathPointSUP[i][1] = target_y;		
				}
				i++;
			}

			PathPointSUP[i][0] = target_x;
			PathPointSUP[i][1] = target_y;		
		}
		else if(RunMode == 1)
		{
			PathPointSUP[i][0] = target_Cx ;
			PathPointSUP[i][1] = target_Cy ;	
		}	
	
	
	}


	
	CntPathPointSUP = i+1;



}


void PathPointSEED_Programme(void)
{
	uint8 i,j,num,x,y,step=200;
	int biasx = 0,biasy = 0;
	float s;
	CntPathPointSEED = 0;
	x = current_x,y = current_y;
//	PathPointSEED[CntPathPointSEED][0] = 0;
//	PathPointSEED[CntPathPointSEED][1] = 0;
//	CntPathPointSEED++;
	for(i = 0;i<CntPathPointSUP;i++)
	{
		s = sqrt(pow((PathPointSUP[i][0] - x),2) +  pow((PathPointSUP[i][1] - y),2)) * 200;
		num =  s / step;
		for(j=0;j<num;j++)
		{
			PathPointSEED[CntPathPointSEED][0] = (PathPointSUP[i][0] - x)*200  * (j + 1)*step/ s + biasx;
			PathPointSEED[CntPathPointSEED][1] = (PathPointSUP[i][1] - y)*200  * (j + 1)*step/ s + biasy;	
			CntPathPointSEED++;
		}
		if(PathPointSEED[CntPathPointSEED-1][0]!=(PathPointSUP[i][0] - x)*200)
		{
			PathPointSEED[CntPathPointSEED][0] = (PathPointSUP[i][0] - x)*200 + biasx;
			PathPointSEED[CntPathPointSEED][1] = (PathPointSUP[i][1] - y)*200 + biasy;	
			CntPathPointSEED++;
		}	
		biasx = (PathPointSUP[i][0] - x)*200,biasy = (PathPointSUP[i][1] - y)*200;
		x = PathPointSUP[i][0] , y = PathPointSUP[i][1];
		
	}
}

