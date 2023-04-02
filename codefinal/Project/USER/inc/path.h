#ifndef __PATH_H
#define __PATH_H

#include "headfile.h"
#include "math.h"
#include "time.h"

void DistanceMatrix_Calculate(void);
int16 Distance_Calculate(uint8 Path[]);
uint8* Generate_NewPath(uint8 Path0[]);
void Find_BestWay(void);
void Find_BestPoint(float x,float y);

#endif