#include "imageprocess.h"

uint8 PositionColumn;
int16 BottomWhiteCount;
uint8 PhototThreshold;
uint8 photoflag;
uint8 Bin_Image[120][188]; 
uint8 Threshold;
uint8 Above_Left[2];
uint8 Above_Right[2];
uint8 Above_TurnPoint[2];
uint8 Above_TurnPresence[2];
uint8 Above_Point[188][2];
uint8 Above_Centre[2];
uint8 Bottom_Point[188][2];
uint8 Bottom_Angular[2];
uint8 bottom_cnt;
uint8 above_cnt;
uint8 line_start;
uint8 line_end;
uint8 line_threshold;
int16 WhiteCnt;

void Bin_Image2(void)
{
	int i,j;
//	if(locate_state == YellowCorrect)
//		Threshold = 140;
//	else 
//		Threshold = 190;
	for(i=0;i<MT9V03X_H;i++)
	{
		for(j=0;j<MT9V03X_W;j++)
		{
			if(mt9v03x_image[i][j] > Threshold)
			{
				Bin_Image[i][j] = 255;
			}
			else
			{
				Bin_Image[i][j] = 0;
			}
		}
	}
}


void Clear_YellowBorder(void)
{
	int i,j;
	uint8 tmp;	
	uint8 RowWhiteCnt,RowLeft,RowRight,RowCentre;
	
	for(i=0;i<MT9V03X_W;i++)
	{
		j = 59;
		if(Bin_Image[j][i] == 0 && Bin_Image[j][i+1] == 255)
		{
			RowLeft = i+1;
			while(1)
			{
				if(Bin_Image[j][i+1] == 255)
				{
					RowWhiteCnt++;
					i++;
					if(i > 188)
						break;
				}
				else
				{
					break;
				}
			}
			RowRight = i;
			
			if(RowWhiteCnt < 30)
			{
				ips114_showuint8(200,5,RowWhiteCnt);
				tmp = i;
				for(j=60;j<MT9V03X_H;j++)
				{
					RowCentre = (RowLeft + RowRight)/2;
					i = RowCentre;
					if(Bin_Image[j][i] == 255)
						Bin_Image[j][i] = 0;
					//clear left
					while(1)
					{
						if(Bin_Image[j][i-1] == 255)
						{
							Bin_Image[j][i-1] = 0;
							i--;
							if(i < 0)
								break;
						}
						else
						{
							break;
						}
					}
					RowLeft = i;
					
					//clear right
					i = RowCentre;
					while(1)
					{
						if(Bin_Image[j][i+1] == 255)
						{
							Bin_Image[j][i+1] = 0;
							i++;
							if(i > 187)
								break;
						}
						else
						{
							break;
						}		
					
					}
					RowRight = i;
				
				}
				i = tmp;
			}
			RowWhiteCnt = 0;

		
		}
	
	}	



}


void Bottom_Detect(void)
{
	int i,j;
	uint8 tmp,index;
	bottom_cnt = 0;
	for(i=0;i<MT9V03X_W;i++)
		for(j=119;j>=60;j--)
		{
			if(Bin_Image[j][i] == 0 && Bin_Image[j+1][i] == 255)
			{
				Bottom_Point[bottom_cnt][0] = j+1;
				Bottom_Point[bottom_cnt][1] = i;
				bottom_cnt++;
				break;
			}		
		}
	if(bottom_cnt > 0)
	{
		tmp = Bottom_Point[0][0];
		index = 0;
		for(i=1;i<bottom_cnt;i++)
		{
			if(Bottom_Point[i][0] > tmp)
			{
				tmp = Bottom_Point[i][0];
				index = i;
			}
		}
		Bottom_Angular[0] = Bottom_Point[index][0];
		Bottom_Angular[1] = Bottom_Point[index][1];			
	}	


}

void Phtot_Detect(void)
{
	uint8 i,j;
	BottomWhiteCount = 0;
	for(i=0;i<MT9V03X_W;i++)
	{
		for(j=60;j<MT9V03X_H;j++)
		{
			if(Bin_Image[j][i] == 255)
			{
				BottomWhiteCount++;			
			}
		}
	}	
}

void Above_Detect(void)
{
	int i,j;
	uint8 tmp;
	uint8 ColumnWhiteCnt;
	above_cnt = 0;
	ColumnWhiteCnt = 0;
	int16 AboveWhiteCount = 0;
	int16 LeftWhiteCount = 0,RightWhiteCount = 0;
	WhiteCnt = 0;
	float k[188];
	BottomWhiteCount = 0;

	for(i=0;i<MT9V03X_W;i++)
	{
		for(j=50;j<MT9V03X_H;j++)
		{
			if(Bin_Image[j][i] == 255)
			{
				BottomWhiteCount++;			
			}
		}
	}
	for(i=0;i<MT9V03X_W;i++)
	{
		if(Bin_Image[60][i] == 255)
			WhiteCnt++;
	}
//	ips114_showuint8(200,0,WhiteCnt);
	if(WhiteCnt > 5)
	{
			Above_Centre[0] = 60;
			Above_Centre[1] = 94;	
			return;
	}	
	if(BottomWhiteCount <= 4)
	{
			Above_Centre[0] = 100;
			Above_Centre[1] =  94;
	}
	else	
	{
		for(i=0;i<MT9V03X_W;i++)
			for(j=50;j<MT9V03X_H;j++)
			{
				if(Bin_Image[j-1][i] == 0 && Bin_Image[j][i] == 255)
				{
					Above_Point[above_cnt][0] = j;
					Above_Point[above_cnt][1] = i;
					above_cnt++;
					break;
				}		
			}
		if(above_cnt > 0)
		{
			Above_Centre[0] = Above_Point[above_cnt/2][0];
			Above_Centre[1] = Above_Point[above_cnt/2][1];			
		}
			
	}
			

//	Above_Left[0] = Above_Point[0][0];
//	Above_Left[1] = Above_Point[0][1];
//		
//	Above_Right[0] = Above_Point[above_cnt-1][0];
//	Above_Right[1] = Above_Point[above_cnt-1][1];
			
			
			
//	line_start = 0;
//	line_end = above_cnt - 1;
		
//	Above_TurnPoint[0] = Above_Point[0][0];
//	for(i=0;i<above_cnt-1;i++)
//	{
//		if(Above_TurnPoint[0]>Above_Point[i+1][0])
//		{
//			Above_TurnPoint[0] = Above_Point[i+1][0];
//			Above_TurnPoint[1] = Above_Point[i+1][1];
//		}
//	}
	
//	for(i=0;i<above_cnt;i++)
//	{
//		k[i] = acos(abs(Above_TurnPoint[1] - Above_Point[i][1])/sqrt(pow(Above_TurnPoint[1] - Above_Point[i][1],2) + pow(Above_TurnPoint[0] - Above_Point[i][0],2)))*180/3.1415926;
//	}
//	for(i=0;i<above_cnt-1;i++)
//	{
//		if(k[i]>=line_threshold&&k[i+1]<=line_threshold)
//		{
//			line_start = i;
//			break;
//		}
//	}
//	for(i=0;i<above_cnt-1;i++)
//	{
//		if(k[i]<=line_threshold&&k[i+1]>=line_threshold)
//		{
//			line_end = i;
//			break;
//		}
//	}
//	if(WhiteCount >= 5)
//	{
//		Above_Centre[0] = Above_Point[above_cnt/2][0];
//		Above_Centre[1] = Above_Point[above_cnt/2][1];	
//	}




//	if(AboveWhiteCount > BottomWhiteCount)
//	{
//		for(i=0;i<94;i++)
//			for(j=0;j<50;j++)
//				if(Bin_Image[j][i] == 255)
//					LeftWhiteCount++;
//		for(i=94;i<MT9V03X_W;i++)
//			for(j = 0;j<50;j++)
//				if(Bin_Image[j][i] == 255)
//					RightWhiteCount++;
//		if(LeftWhiteCount > RightWhiteCount)
//		{
//			Above_Centre[0] = 70;
//			Above_Centre[1] =  70;
//		}
//		if(LeftWhiteCount < RightWhiteCount)
//		{
//			Above_Centre[0] = 70;
//			Above_Centre[1] =  110;
//		}
//	}



//	if(WhiteCount <= 5)
//	{
//		Above_Centre[1] = 92;
//		Above_Centre[0] = 100;
//	}
	//	draw above line
//	for(i=0;i<=above_cnt;i++)
//		ips114_drawpoint(Above_Point[i][1],Above_Point[i][0],RED);
	
	
//	ips114_showuint8(200,1,Above_Centre[0]);
//	ips114_showuint8(200,2,Above_Centre[1]);
	
	
	
//	ips114_drawpoint(Above_TurnPoint[1],Above_TurnPoint[0],BLUE);
//	ips114_drawpoint(Above_TurnPoint[1]-1,Above_TurnPoint[0],BLUE);
//	ips114_drawpoint(Above_TurnPoint[1]+1,Above_TurnPoint[0],BLUE);
//	ips114_drawpoint(Above_TurnPoint[1],Above_TurnPoint[0]-1,BLUE);
//	ips114_drawpoint(Above_TurnPoint[1],Above_TurnPoint[0]+1,BLUE);
	//	
	
}

void Above_Detect1(void)
{
	int i,j;
	uint8 tmp,index;
	uint8 ColumnWhiteCnt;
	above_cnt = 0;
	ColumnWhiteCnt = 0;
	int16 AboveWhiteCount = 0;
	int16 LeftWhiteCount = 0,RightWhiteCount = 0;
	WhiteCnt = 0;
	float k[188];
	BottomWhiteCount = 0;
	
	
	
	for(i=0;i<MT9V03X_H;i++)
	{
		for(j=0;j<MT9V03X_W;j++)
		{
			if(mt9v03x_image[i][j] > 140)
			{
				Bin_Image[i][j] = 255;
			}
			else
			{
				Bin_Image[i][j] = 0;
			}
		}
	}
	
	for(i=0;i<MT9V03X_W;i++)
	{
		for(j=80;j<MT9V03X_H;j++)
		{
			if(Bin_Image[j][i] == 255)
			{
				BottomWhiteCount++;			
			}
		}
	}
	if(BottomWhiteCount > 0)
	{
		for(i=0;i<MT9V03X_W;i++)
			for(j=80;j<MT9V03X_H;j++)
			{
				if(Bin_Image[j-1][i] == 0 && Bin_Image[j][i] == 255)
				{
					Above_Point[above_cnt][0] = j;
					Above_Point[above_cnt][1] = i;
					above_cnt++;
					break;
				}		
			}
		if(above_cnt > 0)
		{
			tmp = Above_Point[0][0];
			index = 0;
			for(i=1;i<above_cnt;i++)
			{
				if(Above_Point[i][0] > tmp)
				{
					tmp = Above_Point[i][0];
					index = i;
				}
			}
			Above_Centre[0] = Above_Point[index][0];
			Above_Centre[1] = Above_Point[index][1];			
		}	
	
	}
	else
	{
		Above_Centre[0] = 80;
	}
}

void CorrectPosition(void)
{
	uint8 i,j;
	above_cnt = 0;
	for(i=0;i<MT9V03X_H;i++)
	{
		for(j=0;j<MT9V03X_W;j++)
		{
			if(mt9v03x_image[i][j] > 190)
			{
				Bin_Image[i][j] = 255;
			}
			else
			{
				Bin_Image[i][j] = 0;
			}
		}
	}	
	
	BottomWhiteCount = 0;
	for(i=0;i<MT9V03X_W;i++)
	{
		for(j=60;j<MT9V03X_H;j++)
		{
			if(Bin_Image[j][i] == 255)
			{
				BottomWhiteCount++;			
			}
		}
	}
	
	for(i=0;i<MT9V03X_W;i++)
	{
		if(Bin_Image[60][i] == 255)
			WhiteCnt++;
	}
//	ips114_showuint8(200,0,WhiteCnt);
	if(WhiteCnt > 5)
	{
			Above_Centre[0] = 60;
			Above_Centre[1] = 92;	
			return;
	}	
	if(BottomWhiteCount <= 4)
	{
			Above_Centre[0] = 100;
			Above_Centre[1] =  78;
	}
	else	
	{
		for(i=0;i<MT9V03X_W;i++)
			for(j=60;j<MT9V03X_H;j++)
			{
				if(Bin_Image[j-1][i] == 0 && Bin_Image[j][i] == 255)
				{
					Above_Point[above_cnt][0] = j;
					Above_Point[above_cnt][1] = i;
					above_cnt++;
					break;
				}		
			}
		if(above_cnt > 0)
		{
			Above_Centre[0] = Above_Point[above_cnt/2][0];
			Above_Centre[1] = Above_Point[above_cnt/2][1];			
		}
			
	}
			
}


