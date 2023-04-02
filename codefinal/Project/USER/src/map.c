#include "map.h"
uint8 BlackThres;
uint8 image[120][188]; 
uint8 flagimage[120][188];
uint8 leftdot;
uint8 bottomdot;
uint8 TopLeft[2];
uint8 BottomRight[2];
uint8 circlecount = 0;
uint8 CircleCenter[20][2];
uint8 coordinate_list[20][2]; 
uint8 map[20][2]; 

uint8 ConnectDomainIndex;
uint8 CDBdotCnt;
uint8 offset4[4][2];
uint16 Rectangledot;

void Bin_image(void)
{
	int i,j;
	for(i=0;i<MT9V03X_H;i++)
	{
		for(j=0;j<MT9V03X_W;j++)
		{
			if(mt9v03x_image[i][j] > BlackThres)
			{
				image[i][j] = 255;
			}
			else
			{
				image[i][j] = 0;
			}
		}
	}


}

void Noise_Filter(void)
{
	int i,j;
	for(i=1;i<MT9V03X_H-1;i++)
		for(j=1;j<MT9V03X_W-1;j++)
		{
			if(image[i][j] ==0)
			{
				if(image[i-1][j]==255&&image[i+1][j]==255&&image[i][j+1]==255&&image[i][j-1]==255)
				{
					image[i][j] = 255;
					continue;
				}
			
			}
		}
	for(j=0;j<MT9V03X_W;j++)		//top
	{
		if(image[1][j] == 0)
			ClearBlackNoise(1,j);
	}
	for(j=0;j<MT9V03X_W;j++)		//bottom
	{
		if(image[118][j] == 0)
			ClearBlackNoise(118,j);
	}
	for(i=0;i<MT9V03X_H;i++)		//left
	{
		if(image[i][1] == 0)
			ClearBlackNoise(i,1);
	}
	for(i=0;i<MT9V03X_H;i++)		//left
	{
		if(image[i][186] == 0)
			ClearBlackNoise(i,186);
	}
	
}

void ClearBlackNoise(uint8 seed_row,uint8 seed_column)
{
	uint8 neighbour_row,neighbour_column;
	uint8 i;
	image[seed_row][seed_column] = 255;
	for(i=0;i<4;i++)
	{
		neighbour_row = seed_row + offset4[i][0];
		neighbour_row = ( (neighbour_row) < (0)  ? (0) : ( (neighbour_row) > (119) ? (119) : (neighbour_row) ) );
		neighbour_column = seed_column + offset4[i][1];
		neighbour_column = ( (neighbour_column) < (0)  ? (0) : ( (neighbour_column) > (187) ? (187) : (neighbour_column) ) );
		if(image[neighbour_row][neighbour_column]==0)
			ClearBlackNoise(neighbour_row,neighbour_column);
		else
			continue;
	}	
}


void Rectangle_Detect(void)
{
	int i,j;
	leftdot = 0;
	memset(flagimage,0,sizeof(flagimage));	
	for(i=3;i<120;i++)
	{
		for(j=1;j<40;j++)
		{
			if(image[i][j-1]==255&&image[i][j]==0&&image[i-1][j]==255&&image[i-2][j]==255&&leftdot==0)
			{
				Rectangledot = 0;
				Rectangle_seed(i,j);
				if(Rectangledot > 30)
				{
					TopLeft[0] = i;
					TopLeft[1] = j;
					leftdot = leftdot + 1;	
					ips114_showuint16(200,1,Rectangledot);					
				}
				break;
			}
		}
		if(leftdot!=0)
			break;
	}
	bottomdot = 0;
	memset(flagimage,0,sizeof(flagimage));	
	for(i=116;i>5;i--)
	{
		for(j = 186;j>60;j--)
		{
			if(image[i][j+1]==255&&image[i][j]==0&&image[i+1][j]==255&&image[i+2][j]==255&&bottomdot==0)
			{
				Rectangledot = 0;
				Rectangle_seed(i,j);
				if(Rectangledot > 30)
				{
					BottomRight[0] = i;
					BottomRight[1] = j;			
					bottomdot = bottomdot + 1;		
					ips114_showuint16(200,2,Rectangledot);
				}				
				break;
			}
		}
		if(bottomdot!=0)
			break;		
	}


}
void Rectangle_seed(uint8 seed_row,uint8 seed_column)
{
	uint8 neighbour_row,neighbour_column;
	uint8 i;
	Rectangledot++;
	flagimage[seed_row][seed_column] = 1;
	for(i=0;i<4;i++)
	{
		neighbour_row = seed_row + offset4[i][0];
		neighbour_row = ( (neighbour_row) < (0)  ? (0) : ( (neighbour_row) > (119) ? (119) : (neighbour_row) ) );
		neighbour_column = seed_column + offset4[i][1];
		neighbour_column = ( (neighbour_column) < (0)  ? (0) : ( (neighbour_column) > (187) ? (187) : (neighbour_column) ) );
		if(image[neighbour_row][neighbour_column]==0&&flagimage[neighbour_row][neighbour_column]==0)
			Rectangle_seed(neighbour_row,neighbour_column);
		else
			continue;
	}	
}



void Find_Circle(void)
{	
	int i,j,k,l,p;
	circlecount = 0;
	uint8 bias;
	uint8 BlackCnt=0;
	uint8 RepeatFlag = 0;
	for(i=TopLeft[0]+4;i<BottomRight[0]-4;i++)
	{
		for(j=TopLeft[1]+4;j<BottomRight[1]-4;j++)
		{
			if(image[i][j]==0)
			{
				BlackCnt = 0;
				for(k = i-2;k<=i+2;k++)
					for(l =j-2;l<=j+2;l++)
					{
						if(image[k][l] == 0)
							BlackCnt ++;
					
					}
				if((image[i-1][j-1]+image[i][j-1]+image[i+1][j-1]+image[i-1][j]+image[i+1][j]+image[i-1][j+1]+image[i][j+1]+image[i+1][j])==0)
				{
					if((image[i-2][j]+image[i][j-2]+image[i+2][j]+image[i][j+2])<=2*255)
					{
						for(p = 0;p<circlecount;p++)
						{
							bias = sqrt(pow((i - CircleCenter[p][0]),2) + pow((j - CircleCenter[p][1]),2));
							if(bias<=10)
							{
								RepeatFlag = 1;
								break;
							}
						
						}
						if(RepeatFlag == 1)
						{
							RepeatFlag = 0;
							continue;
						}
						CircleCenter[circlecount][0] = i;
						CircleCenter[circlecount][1] = j;
						circlecount = circlecount + 1;	
	
					}
		
				}
			}
		}
	}



}

void Find_Circle1(void)
{
	int i,j;
	circlecount = 0;
	ConnectDomainIndex = 0;
	memset(flagimage,0,sizeof(flagimage));		//reset
	for(i=TopLeft[0]+4;i<BottomRight[0]-4;i++)
	{
		for(j=TopLeft[1]+4;j<BottomRight[1]-4;j++)
		{
			if(image[i][j]==0&&flagimage[i][j]==0)
			{
				ConnectDomainIndex++;
				CDBdotCnt = 0;
				recursive_seed(i,j);
				if(CDBdotCnt > 0)
				{
					CircleCenter[circlecount][0] = i;
					CircleCenter[circlecount][1] = j;
					circlecount = circlecount + 1;					
				}
			}
		}
	}		

}


void recursive_seed(uint8 seed_row,uint8 seed_column)
{
	uint8 neighbour_row,neighbour_column;
	uint8 i;
	CDBdotCnt++;
	flagimage[seed_row][seed_column] = ConnectDomainIndex;
	for(i=0;i<4;i++)
	{
		neighbour_row = seed_row + offset4[i][0];
		neighbour_row = ( (neighbour_row) < (0)  ? (0) : ( (neighbour_row) > (119) ? (119) : (neighbour_row) ) );
		neighbour_column = seed_column + offset4[i][1];
		neighbour_column = ( (neighbour_column) < (0)  ? (0) : ( (neighbour_column) > (187) ? (187) : (neighbour_column) ) );
		if(image[neighbour_row][neighbour_column]==0&&flagimage[neighbour_row][neighbour_column]==0)
			recursive_seed(neighbour_row,neighbour_column);
		else
			continue;
	}
}

void Draw_CircleCenter(void)
{
	int i;
	for(i=0;i<circlecount;i++)
	{
		ips114_drawpoint(CircleCenter[i][1],CircleCenter[i][0],GREEN);
		ips114_drawpoint(CircleCenter[i][1]-1,CircleCenter[i][0],GREEN);
		ips114_drawpoint(CircleCenter[i][1]-2,CircleCenter[i][0],GREEN);
		ips114_drawpoint(CircleCenter[i][1]+1,CircleCenter[i][0],GREEN);
		ips114_drawpoint(CircleCenter[i][1]+2,CircleCenter[i][0],GREEN);
		ips114_drawpoint(CircleCenter[i][1],CircleCenter[i][0]-1,GREEN);
		ips114_drawpoint(CircleCenter[i][1],CircleCenter[i][0]-2,GREEN);
		ips114_drawpoint(CircleCenter[i][1],CircleCenter[i][0]+1,GREEN);
		ips114_drawpoint(CircleCenter[i][1],CircleCenter[i][0]+2,GREEN);
	}

}


void Draw_Rectangle(void)
{
	int i;
	//Draw Top line
	for(i=TopLeft[1];i<=BottomRight[1];i++)
	{
		ips114_drawpoint(i,TopLeft[0],RED);
		ips114_drawpoint(i,TopLeft[0]+1,RED);
	}
	//Draw bottom line
	for(i=TopLeft[1];i<=BottomRight[1];i++)
	{
		ips114_drawpoint(i,BottomRight[0]-1,RED);
		ips114_drawpoint(i,BottomRight[0],RED);
	}
	//Draw left line
	for(i=TopLeft[0];i<=BottomRight[0];i++)
	{
		ips114_drawpoint(TopLeft[1],i,RED);
		ips114_drawpoint(TopLeft[1]+1,i,RED);
	}	
	//Draw right line
	for(i=TopLeft[0];i<=BottomRight[0];i++)
	{
		ips114_drawpoint(BottomRight[1],i,RED);
		ips114_drawpoint(BottomRight[1]-1,i,RED);
	}	

}

void Find_CircleCenter(void)
{
	int i,j;
	uint8 chord_left,chord_right,chord_centre;
	uint8 diameter_bottom,diameter_top,diameter_centre;
	for(i=0;i<circlecount;i++)
	{
		j = CircleCenter[i][1];
		while(1)
		{
			if(image[CircleCenter[i][0]][j] == 0 && image[CircleCenter[i][0]][j-1]==255)
			{
				chord_left = j;
				break;
			}
			j--;
		}
		
		j = CircleCenter[i][1];
		while(1)
		{
			if(image[CircleCenter[i][0]][j] == 0 && image[CircleCenter[i][0]][j+1]==255)
			{
				chord_right = j;
				break;
			}
			j++;
			if(j>=186)
				break;
		}
		chord_centre = (chord_left + chord_right)/2;
		
		j = CircleCenter[i][0];
		while(1)
		{
			if(image[j][chord_centre] == 0 && image[j - 1][chord_centre]==255)
			{
				diameter_top = j;
				break;
			}
			j--;
			if(j<=1)
				break;
		}		
		
		j = CircleCenter[i][0];
		while(1)
		{
			if(image[j][chord_centre] == 0 && image[j + 1][chord_centre]==255)
			{
				diameter_bottom = j;
				break;
			}
			j++;
			if(j>=118)
				break;
		}		
		diameter_centre = (diameter_top + diameter_bottom)/2;
		CircleCenter[i][0] = diameter_centre;
		CircleCenter[i][1] = chord_centre;
			
	}

}



void CoordinateConvert(void)
{
	uint8 length,width;
	uint8 i;
	int16 temp;
	length = BottomRight[1] - TopLeft[1]-2;
	width  = BottomRight[0] - TopLeft[0]-2;
//	coordinate_list[0][0] = 1;
//	coordinate_list[0][1] = 1;
	for(i=0;i<circlecount;i++)
	{
		temp = (CircleCenter[i][1] -  TopLeft[1] -1)*35;
		coordinate_list[i][0] = (temp - 1)/length + 1;
		temp = (BottomRight[0] - CircleCenter[i][0] -1)*25;
		coordinate_list[i][1] = (temp - 1)/width + 1;
	
	}
//	coordinate_list[circlecount+1][0] = 1;
//	coordinate_list[circlecount+1][1] = 1;
}

void Map_Show(void)
{
	uint8 i;
	for(i=0;i<8;i++)
	{
		ips114_showuint8(0,i,coordinate_list[i][0]);
		ips114_showuint8(40,i,coordinate_list[i][1]);
	}
	for(i=8;i<12;i++)
	{
		ips114_showuint8(80,i-8,coordinate_list[i][0]);
		ips114_showuint8(120,i-8,coordinate_list[i][1]);
	}




}

