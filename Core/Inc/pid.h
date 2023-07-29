#ifndef __PID_H
#define __PID_H

#include "system.h"

typedef struct{
    float target;
    float bias;
    float previous_bias;
    float integral;
    float integrate_limit;
    float Kp, Ki, Kd;
} PID;

void PID_Init(PID *);

void PID_Set_Kp(PID *, float);
void PID_Set_Ki(PID *, float);
void PID_Set_Kd(PID *, float);

void PID_Set_Coefficients(PID *, float, float, float);

void PID_Set_Target(PID *, float);

void PID_Set_IntegrateLimit(PID *, float);

void Update_PID(PID *, float current, float *output);
void Update_PID_DerivKnown(PID *, float current, float derivative, float *output);

#endif
