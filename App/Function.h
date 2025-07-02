#ifndef __Function_H
#define __Function_H

#include "stm32f10x.h"

void ScreenOrigin_PositionSet(void);

void Material_FeatureSel(void);
void Material_Adjust(void);
void Material_Select(void);

void Material_PTZposition(void);
void Init_PTZ_Position(void);
long CBM_D_Value(long set_val,long read_val,unsigned char error_val);
void Init_PTZ_PID(long set_valx,long set_valy);

#endif

