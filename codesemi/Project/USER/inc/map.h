#ifndef __MAP_H
#define __MAP_H

#include "headfile.h"
#include "math.h"

void Find_Circle1(void);
void recursive_seed(uint8 seed_row,uint8 seed_column);
void Bin_image(void);
void Rectangle_Detect(void);
void Rectangle_seed(uint8 seed_row,uint8 seed_column);
void Draw_Rectangle(void);
void Find_Circle(void);
void Find_CircleCenter(void);
void CoordinateConvert(void);
void Noise_Filter(void);
void ClearBlackNoise(uint8 seed_row,uint8 seed_column);
void Draw_CircleCenter(void);
void Map_Show(void);
#endif