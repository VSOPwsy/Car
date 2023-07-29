#include "motor.h"

void Motor_Init(void)
{
    HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_4);

    HAL_TIM_Encoder_Start(&htim3,TIM_CHANNEL_ALL);
    HAL_TIM_Encoder_Start(&htim4,TIM_CHANNEL_ALL);
    HAL_TIM_Encoder_Start(&htim5,TIM_CHANNEL_ALL);
    HAL_TIM_Encoder_Start(&htim8,TIM_CHANNEL_ALL);
}

void Smoothing(float Vx, float Vy, float Vz)
{
    float step=0.01;

	if	   (Vx>0) 	smooth_control.VX+=step;
	else if(Vx<0)	smooth_control.VX-=step;
	else if(Vx==0)	smooth_control.VX=smooth_control.VX*0.9f;
	
	if	   (Vy>0)   smooth_control.VY+=step;
	else if(Vy<0)	smooth_control.VY-=step;
	else if(Vy==0)	smooth_control.VY=smooth_control.VY*0.9f;
	
	if	   (Vz>0) 	smooth_control.VZ+=step;
	else if(Vz<0)	smooth_control.VZ-=step;
	else if(Vz==0)	smooth_control.VZ=smooth_control.VZ*0.9f;
	
	smooth_control.VX=target_limit_float(smooth_control.VX,-float_abs(Vx),float_abs(Vx));
	smooth_control.VY=target_limit_float(smooth_control.VY,-float_abs(Vy),float_abs(Vy));
	smooth_control.VZ=target_limit_float(smooth_control.VZ,-float_abs(Vz),float_abs(Vz));
}
