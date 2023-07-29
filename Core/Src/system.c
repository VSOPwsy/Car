#include "system.h"

uint32_t Move_X, Move_Y, Move_Z;
float Kp=300,Ki=300; 
Motor_Parameter Motor_LeftFront, Motor_LeftRear, Motor_RightFront, Motor_RightRear;
Smooth_Control smooth_control;

float target_limit_float(float insert, float low, float high)
{
    if (insert < low)
        return low;
    else if (insert > high)
        return high;
    else
        return insert;	
}

int target_limit_int(int insert, int low, int high)
{
    if (insert < low)
        return low;
    else if (insert > high)
        return high;
    else
        return insert;	
}

float float_abs(float insert)
{
    if (insert < 0)
        return -insert;
    else
        return insert;	
}
