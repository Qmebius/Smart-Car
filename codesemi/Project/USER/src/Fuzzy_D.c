#include "Fuzzy_D.h"

//float DError;	//distance error


float Fuzzy_Speed(float DError)
{
	uint8 PE;
	float DErrorRule[7] = {30,60,100,200,1000,1500,2000};	
	float DFuzzy[2] = {0.0,0.0};
	int16 SpeedRule[7] = {30,100,100,100,100,100,100};
	float Yspeed;
	if(abs(DError) <= 10)
	{
		return 0;
	}
	if(DError <= DErrorRule[0])
	{
		PE = 0;
		DFuzzy[0] = 1;	
	}
	else if(DError <= DErrorRule[1])
	{
		PE = 0;
		DFuzzy[0] = (DErrorRule[1] - DError)/(DErrorRule[1] - DErrorRule[0]);
	}
	else if(DError <= DErrorRule[2])
	{
		PE = 1;
		DFuzzy[0] = (DErrorRule[2] - DError)/(DErrorRule[2] - DErrorRule[1]);
	}
	else if(DError <= DErrorRule[3])
	{
		PE = 2;
		DFuzzy[0] = (DErrorRule[3] - DError)/(DErrorRule[3] - DErrorRule[2]);
	}	
	else if(DError <= DErrorRule[4])
	{
		PE = 3;
		DFuzzy[0] = (DErrorRule[4] - DError)/(DErrorRule[4] - DErrorRule[3]);
	}	
	else if(DError <= DErrorRule[5])
	{
		PE = 4;
		DFuzzy[0] = (DErrorRule[5] - DError)/(DErrorRule[5] - DErrorRule[4]);
	}	
	else if(DError <= DErrorRule[6])
	{
		PE = 5;
		DFuzzy[0] = (DErrorRule[6] - DError)/(DErrorRule[6] - DErrorRule[5]);
	}
	else
	{
		PE = 5;
		DFuzzy[0] = 0;
	}
	DFuzzy[1] = 1 - DFuzzy[0];
	Yspeed = DFuzzy[0] * SpeedRule[PE] + DFuzzy[1] * SpeedRule[PE+1];
	return Yspeed;
}


/*float Fuzzy_Speed(float DError)
{
	uint8 PE;
	float DErrorRule[7] = {30,60,100,600,1000,1500,2000};	
	float DFuzzy[2] = {0.0,0.0};
	int16 SpeedRule[7] = {30,60,100,100,100,100,100};
	float Yspeed;
	if(abs(DError) <= 10)
	{
		return 0;
	}
	if(DError <= DErrorRule[0])
	{
		PE = 0;
		DFuzzy[0] = 1;	
	}
	else if(DError <= DErrorRule[1])
	{
		PE = 0;
		DFuzzy[0] = (DErrorRule[1] - DError)/(DErrorRule[1] - DErrorRule[0]);
	}
	else if(DError <= DErrorRule[2])
	{
		PE = 1;
		DFuzzy[0] = (DErrorRule[2] - DError)/(DErrorRule[2] - DErrorRule[1]);
	}
	else if(DError <= DErrorRule[3])
	{
		PE = 2;
		DFuzzy[0] = (DErrorRule[3] - DError)/(DErrorRule[3] - DErrorRule[2]);
	}	
	else if(DError <= DErrorRule[4])
	{
		PE = 3;
		DFuzzy[0] = (DErrorRule[4] - DError)/(DErrorRule[4] - DErrorRule[3]);
	}	
	else if(DError <= DErrorRule[5])
	{
		PE = 4;
		DFuzzy[0] = (DErrorRule[5] - DError)/(DErrorRule[5] - DErrorRule[4]);
	}	
	else if(DError <= DErrorRule[6])
	{
		PE = 5;
		DFuzzy[0] = (DErrorRule[6] - DError)/(DErrorRule[6] - DErrorRule[5]);
	}
	else
	{
		PE = 5;
		DFuzzy[0] = 0;
	}
	DFuzzy[1] = 1 - DFuzzy[0];
	Yspeed = DFuzzy[0] * SpeedRule[PE] + DFuzzy[1] * SpeedRule[PE+1];
	return Yspeed;
}
*/




