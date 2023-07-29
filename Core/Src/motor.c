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


// ������ǰ�������γɵĽǶ�(��)���ٶȣ�m/s�������������ķ�ֵ�PWMռ�ձ�, �־�ΪWHEEL_DIAMETER�����ٶ�д��������Motor_LeftFront_Speed�ı���
void Motor_CalculateSpeed(uint32_t angle, float speed)
{
    // �ٶȽ���
    Motor_LeftFront_Speed  = (uint32_t)(speed * sinf((angle + 45) * M_PI / 180) * 1000);
    Motor_LeftRear_Speed   = (uint32_t)(speed * cosf((angle + 45) * M_PI / 180) * 1000);
    Motor_RightFront_Speed = (uint32_t)(speed * cosf((angle + 45) * M_PI / 180) * 1000);
    Motor_RightRear_Speed  = (uint32_t)(speed * sinf((angle + 45) * M_PI / 180) * 1000);
    // �ٶ��޷�
    if (Motor_LeftFront_Speed > 1000)
        Motor_LeftFront_Speed = 1000;
    if (Motor_LeftRear_Speed > 1000)
        Motor_LeftRear_Speed = 1000;
    if (Motor_RightFront_Speed > 1000)
        Motor_RightFront_Speed = 1000;
    if (Motor_RightRear_Speed > 1000)
        Motor_RightRear_Speed = 1000;
    // �ٶ�����
    if (Motor_LeftFront_Speed < 10)
        Motor_LeftFront_Speed = 0;
    if (Motor_LeftRear_Speed < 10)
        Motor_LeftRear_Speed = 0;
    if (Motor_RightFront_Speed < 10)
        Motor_RightFront_Speed = 0;
    if (Motor_RightRear_Speed < 10)
        Motor_RightRear_Speed = 0;
}