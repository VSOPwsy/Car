#ifndef __MOTOR_H
#define __MOTOR_H

#include "system.h"
#include "pid.h"

void Motor_Init(void);
void Smoothing(float Vx, float Vy, float Vz);
void Solve_Speed(float Vx, float Vy, float Vz);

extern PID PID_Motor_LeftFront;
extern PID PID_Motor_RightFront;
extern PID PID_Motor_LeftRear;
extern PID PID_Motor_RightRear;

#endif
