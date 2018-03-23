#ifndef  GPS_H
#define  GPS_H

#include <stdio.h>
#include <math.h>
#include "stm32f10x.h"
#include "usart.h"

#define iPI     0.0174533 //PI/180
void ubx_CheckSum(u8 data);
void change_Pos();
void gps_Get(u8 temp);

#endif
