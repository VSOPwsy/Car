#include "motor.h"


PID PID_Motor_LeftFront;
PID PID_Motor_RightFront;
PID PID_Motor_LeftRear;
PID PID_Motor_RightRear;

void Motor_Init(void)
{
    HAL_TIM_PWM_Start(&LEFTFRONT_MOTOR_PWM,  LEFTFRONT_MOTOR_PWM_CHANNEL);
    HAL_TIM_PWM_Start(&RIGHTFRONT_MOTOR_PWM, RIGHTFRONT_MOTOR_PWM_CHANNEL);
    HAL_TIM_PWM_Start(&LEFTREAR_MOTOR_PWM,   LEFTREAR_MOTOR_PWM_CHANNEL);
    HAL_TIM_PWM_Start(&RIGHTREAR_MOTOR_PWM,  RIGHTREAR_MOTOR_PWM_CHANNEL);

    HAL_TIM_Encoder_Start(&LEFTFRONT_MOTOR_ENCODER_TIM, TIM_CHANNEL_ALL);
    HAL_TIM_Encoder_Start(&RIGHTFRONT_MOTOR_ENCODER_TIM, TIM_CHANNEL_ALL);
    HAL_TIM_Encoder_Start(&LEFTREAR_MOTOR_ENCODER_TIM, TIM_CHANNEL_ALL);
    HAL_TIM_Encoder_Start(&RIGHTREAR_MOTOR_ENCODER_TIM, TIM_CHANNEL_ALL);

    PID_Init(&PID_Motor_LeftFront,  MOTOR_PID_KP, 0, MOTOR_PID_KD);
    PID_Init(&PID_Motor_RightFront, MOTOR_PID_KP, 0, MOTOR_PID_KD);
    PID_Init(&PID_Motor_LeftRear,   MOTOR_PID_KP, 0, MOTOR_PID_KD);
    PID_Init(&PID_Motor_RightRear,  MOTOR_PID_KP, 0, MOTOR_PID_KD);
}

void Smoothing(float Vx, float Vy, float Vz)
{
    float step=0.01;

	if	   (Vx>0) 	smooth_control.VX += step;
	else if(Vx<0)	smooth_control.VX -= step;
	else if(Vx==0)	smooth_control.VX =  smooth_control.VX*0.9f;
	
	if	   (Vy>0)   smooth_control.VY += step;
	else if(Vy<0)	smooth_control.VY -= step;
	else if(Vy==0)	smooth_control.VY = smooth_control.VY*0.9f;
	
	if	   (Vz>0) 	smooth_control.VZ += step;
	else if(Vz<0)	smooth_control.VZ -= step;
	else if(Vz==0)	smooth_control.VZ =  smooth_control.VZ*0.9f;
	
	smooth_control.VX = target_limit_float(smooth_control.VX, -float_abs(Vx), float_abs(Vx));
	smooth_control.VY = target_limit_float(smooth_control.VY, -float_abs(Vy), float_abs(Vy));
	smooth_control.VZ = target_limit_float(smooth_control.VZ, -float_abs(Vz), float_abs(Vz));
}

void Measure_Motor_Speed(void)
{
    // divide by 10ms, that is, times 100
    Motor_LeftFront.Encoder  =  __HAL_TIM_GET_COUNTER(&LEFTFRONT_MOTOR_ENCODER_TIM)  *100*WHEEL_DIAMETER/ENCODER_PRECISION;
    Motor_RightFront.Encoder =  __HAL_TIM_GET_COUNTER(&RIGHTFRONT_MOTOR_ENCODER_TIM) *100*WHEEL_DIAMETER/ENCODER_PRECISION;
    Motor_LeftRear.Encoder   =  __HAL_TIM_GET_COUNTER(&LEFTREAR_MOTOR_ENCODER_TIM)   *100*WHEEL_DIAMETER/ENCODER_PRECISION;
    Motor_RightRear.Encoder  =  __HAL_TIM_GET_COUNTER(&RIGHTREAR_MOTOR_ENCODER_TIM)  *100*WHEEL_DIAMETER/ENCODER_PRECISION;

    __HAL_TIM_SET_COUNTER(&LEFTFRONT_MOTOR_ENCODER_TIM, 0);
    __HAL_TIM_SET_COUNTER(&RIGHTFRONT_MOTOR_ENCODER_TIM, 0);
    __HAL_TIM_SET_COUNTER(&LEFTREAR_MOTOR_ENCODER_TIM, 0);
    __HAL_TIM_SET_COUNTER(&RIGHTREAR_MOTOR_ENCODER_TIM, 0);
}

void Solve_Speed(float Vx, float Vy, float Vz)
{
    if (SMOOTH_CONTROL != 0)
    {
        Smoothing(Vx, Vy, Vz);
        Vx = smooth_control.VX;
        Vy = smooth_control.VY;
        Vz = smooth_control.VZ;
    }
    
    Motor_LeftFront.Target  = +Vy+Vx-Vz*(Axle_spacing+Wheel_spacing);
    Motor_RightFront.Target = -Vy+Vx-Vz*(Axle_spacing+Wheel_spacing);
    Motor_LeftRear.Target   = +Vy+Vx+Vz*(Axle_spacing+Wheel_spacing);
    Motor_RightRear.Target  = -Vy+Vx+Vz*(Axle_spacing+Wheel_spacing);

    Motor_LeftFront.Target  = target_limit_float(Motor_LeftFront.Target,  -MOTOR_SPEED_LIMIT , MOTOR_SPEED_LIMIT);
    Motor_RightFront.Target = target_limit_float(Motor_RightFront.Target, -MOTOR_SPEED_LIMIT , MOTOR_SPEED_LIMIT);
    Motor_LeftRear.Target   = target_limit_float(Motor_LeftRear.Target,   -MOTOR_SPEED_LIMIT , MOTOR_SPEED_LIMIT);
    Motor_RightRear.Target  = target_limit_float(Motor_RightRear.Target,  -MOTOR_SPEED_LIMIT , MOTOR_SPEED_LIMIT);
}


void Set_PWM()
{
    __HAL_TIM_SET_COMPARE(&LEFTFRONT_MOTOR_PWM,  LEFTFRONT_MOTOR_PWM_CHANNEL,  Motor_LeftFront.PWM);
    __HAL_TIM_SET_COMPARE(&RIGHTFRONT_MOTOR_PWM, RIGHTFRONT_MOTOR_PWM_CHANNEL, Motor_RightFront.PWM);
    __HAL_TIM_SET_COMPARE(&LEFTREAR_MOTOR_PWM,   LEFTREAR_MOTOR_PWM_CHANNEL,   Motor_LeftRear.PWM);
    __HAL_TIM_SET_COMPARE(&RIGHTREAR_MOTOR_PWM,  RIGHTREAR_MOTOR_PWM_CHANNEL,  Motor_RightRear.PWM);
}

void Update_Motor_PID()
{
    Update_PID_DerivKnown(&PID_Motor_LeftFront,  Motor_LeftFront.Encoder,  0, &(Motor_LeftFront.PWM));
    Update_PID_DerivKnown(&PID_Motor_RightFront, Motor_RightFront.Encoder, 0, &(Motor_RightFront.PWM));
    Update_PID_DerivKnown(&PID_Motor_LeftRear,   Motor_LeftRear.Encoder,   0, &(Motor_LeftRear.PWM));
    Update_PID_DerivKnown(&PID_Motor_RightRear,  Motor_RightRear.Encoder,  0, &(Motor_RightRear.PWM));
}

