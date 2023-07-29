#ifndef __MOTOR_H
#define __MOTOR_H

#include "system.h"

void Motor_Init(void);
void Smoothing(float Vx, float Vy, float Vz);
void Solve_Speed(float Vx, float Vy, float Vz);

#endif
