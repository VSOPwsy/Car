#ifndef __MOTOR_H
#define __MOTOR_H

#include "pid.h"

extern PID PID_Motor_LeftFront;
extern PID PID_Motor_RightFront;
extern PID PID_Motor_LeftRear;
extern PID PID_Motor_RightRear;

void Motor_Init(void);
void Smoothing(float Vx, float Vy, float Vz);
void Measure_Motor_Speed(void);
void Solve_Speed(float Vx, float Vy, float Vz);
void Motor_Set_PWM(void);
void Update_Motor_PID(void);

#endif
