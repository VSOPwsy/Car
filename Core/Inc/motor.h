#ifndef __MOTOR_H
#define __MOTOR_H

#include "system.h"

void Motor_Init(void);
void Smooth_Control(float Vx, float Vy, float Vz);
#endif