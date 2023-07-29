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

void Smooth_Control(float Vx, float Vy, float Vz)
{

}
