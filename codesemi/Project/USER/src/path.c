#include "path.h"

uint8 PointNum;
int16 DistanceMatrix[20][20];
uint8 WayIndex[20];
uint8 Path0[20];
uint8 PointFlag[12];

void DistanceMatrix_Calculate(void)
{
	int i,j;
	for(i=0;i<PointNum;i++)
		for(j=0;j<PointNum;j++)
		{
			DistanceMatrix[i][j] = sqrt(pow((coordinate_list[i][0] - coordinate_list[j][0]),2) + pow(coordinate_list[i][1] - coordinate_list[j][1],2));
		}
}

int16 Distance_Calculate(uint8 Path[])
{
	int16 distance = 0;
	int i,j;
	for(i=0;i<PointNum;i++)
		distance += DistanceMatrix[Path[i]][Path[i+1]];
	return distance;
}



uint8* Generate_NewPath(uint8 Path0[])
{
	float r1,r2,rand;
	uint8 i;
	uint8 k,m,temp;
	static uint8 Path1[20];
	r1 = 0.33;
	r2 = 0.33;
	rand = my_rand(2);
	if(rand < r1)		//倒置法
	{
		k = Rand_Integer(1,PointNum-2);
		m = Rand_Integer(1,PointNum-2);
		if(k > m)
		{
			temp = m;
			m = k;
			k = temp;
		}
		for(i=0;i<k;i++)
			Path1[i] = Path0[i];
		for(i=0;i<=m-k;i++)
			Path1[i+k] = Path0[m-i];
		for(i=m+1;i<PointNum;i++)
			Path1[i] = Path0[i];
		
	}
	else if(rand < r1 + r2) //移位法
	{
		k = Rand_Integer(1,PointNum-2);
		m = Rand_Integer(1,PointNum-2);
		if(k > m)
		{
			temp = m;
			m = k;
			k = temp;
		}
		for(i=0;i<k;i++)
			Path1[i] = Path0[i];
		Path1[k] = Path0[m];
		for(i =k+1;i<=m;i++)
			Path1[i] = Path0[i-1];
		for(i=m+1;i<PointNum;i++)
			Path1[i] = Path0[i];
	}
	else	//交换法
	{
		k = Rand_Integer(1,PointNum-2);
		m = Rand_Integer(1,PointNum-2);		
		for(i=0;i<PointNum;i++)
		{
			Path1[i] = Path0[i];
		}
		Path1[k] = Path0[m];
		Path1[m] = Path0[k];
	}
	return Path1;
}



void Find_BestWay(void)
{
	int16 Distance_Best,distance1,distance0;
	uint8 Path0[20],path_best[20];
	uint8 *Path1;
	int16 i,j;
	
	//Parameter Initialize
	int16 T = 1000;
	int16 L1 = 1000;
	int16 L2 = 300;
	float at = 0.95;
	float e = 1e-30;
//	ips114_showuint8(1,1,1);
	//Calculate Distance 
	DistanceMatrix_Calculate();
//	ips114_showuint8(2,2,2);
	//Initialize Path0
	for(i=0;i<PointNum;i++)
	{
		Path0[i] = i;
	}
	distance0 = Distance_Calculate(Path0);
	Distance_Best = distance0;
	memcpy(path_best,Path0,sizeof(path_best)); // Path_best = Path0
	ips114_showuint8(160,60,2);
	for(i=0;i<L1;i++)
	{
		for(j=0;j<L2;j++)
		{
			Path1 = Generate_NewPath(Path0);
			distance1 = Distance_Calculate(Path1);
			if(distance1 < Distance_Best)
			{
				memcpy(Path0,Path1,sizeof(Path0)); // Path0 = Path1
				distance0 = distance1;
			}
			else if(exp(-(distance1 - distance0)/T) < my_rand(2))
			{
				memcpy(Path0,Path1,sizeof(Path0)); // Path0 = Path1
				distance0 = distance1;
			}
			if(distance0 < Distance_Best)
			{
				Distance_Best = distance0;
				memcpy(path_best,Path0,sizeof(path_best)); // Path_best = Path0
			}
		}
		T = T * at;
		if(T < e)
			break;
	}

	for(i=0;i<8;i++)
	{
		map[i][0] = coordinate_list[path_best[i]][0];
		map[i][1] = coordinate_list[path_best[i]][1];
		ips114_showuint8(0,i,map[i][0]);
		ips114_showuint8(40,i,map[i][1]);
	}
	if(PointNum <= 15)
	{
		for(i=8;i<PointNum;i++)
		{
			map[i][0] = coordinate_list[path_best[i]][0];
			map[i][1] = coordinate_list[path_best[i]][1];
			ips114_showuint8(80,i-8,map[i][0]);
			ips114_showuint8(120,i-8,map[i][1]);
		}
	}
	if(PointNum > 15)
	{
		for(i=8;i<16;i++)
		{
			map[i][0] = coordinate_list[path_best[i]][0];
			map[i][1] = coordinate_list[path_best[i]][1];
			ips114_showuint8(80,i-8,map[i][0]);
			ips114_showuint8(120,i-8,map[i][1]);
		}
		for(i=16;i<PointNum;i++)
		{
			map[i][0] = coordinate_list[path_best[i]][0];
			map[i][1] = coordinate_list[path_best[i]][1];
			ips114_showuint8(160,i-16,map[i][0]);
			ips114_showuint8(200,i-16,map[i][1]);		
		
		}
	}
	
//	ips114_showint16(40,7,Distance_Best);



}


void Find_BestPoint(float x,float y)
{
	uint8 i,index;
	float tmp,distance;
	distance = 9999;
	if(NowCntMapPoint == circlecount)
	{
		next_x = 1;
		next_y = 1;		
		return;
	}
	for(i=0;i<circlecount;i++)
	{
		if(PointFlag[i] == 1)
			continue;
		tmp = sqrt(pow((x - coordinate_list[i][0]),2) + pow((y - coordinate_list[i][1] + 1),2));
		if(distance > tmp)
		{
			distance = tmp;
			index = i;
		}
	
	}
	PointFlag[index] = 1;
	next_x = coordinate_list[index][0];
	next_y = coordinate_list[index][1] - 1;
}