#include "motor.h"

uint32_t Motor_LeftFront_Target_Speed  = 0;
uint32_t Motor_LeftRear_Target_Speed   = 0;
uint32_t Motor_RightFront_Target_Speed = 0;
uint32_t Motor_RightRear_Target_Speed  = 0;

uint32_t Motor_LeftFront_Speed_Previouse  = 0;
uint32_t Motor_LeftRear_Speed_Previouse   = 0;
uint32_t Motor_RightFront_Speed_Previouse = 0;
uint32_t Motor_RightRear_Speed_Previouse  = 0;

void Motor_Output(float speed)
{

    if (speed>0)
    {       
            __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, speed);
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, speed);
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, speed);
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, speed);

			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);

			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);

            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);

            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET);
    }
    else
    {       
            __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, -speed);
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, -speed);
            __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, -speed);
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, -speed);

			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);

			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET);

            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);

            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET);
    }
    }
}


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



// 输入与前进方向形成的角度(°)、速度（m/s），解算麦克纳姆轮的PWM占空比, 轮径为WHEEL_DIAMETER，把速度写入名称如Motor_LeftFront_Speed的变量
void Motor_CalculateSpeed(uint32_t angle, float speed)
{
    // 速度解算
    Motor_LeftFront_Speed  = (uint32_t)(speed * sinf((angle + 45) * M_PI / 180) * 1000);
    Motor_LeftRear_Speed   = (uint32_t)(speed * cosf((angle + 45) * M_PI / 180) * 1000);
    Motor_RightFront_Speed = (uint32_t)(speed * cosf((angle + 45) * M_PI / 180) * 1000);
    Motor_RightRear_Speed  = (uint32_t)(speed * sinf((angle + 45) * M_PI / 180) * 1000);
    // 速度限幅
    if (Motor_LeftFront_Speed > 1000)
        Motor_LeftFront_Speed = 1000;
    if (Motor_LeftRear_Speed > 1000)
        Motor_LeftRear_Speed = 1000;
    if (Motor_RightFront_Speed > 1000)
        Motor_RightFront_Speed = 1000;
    if (Motor_RightRear_Speed > 1000)
        Motor_RightRear_Speed = 1000;
    // 速度死区
    if (Motor_LeftFront_Speed < 10)
        Motor_LeftFront_Speed = 0;
    if (Motor_LeftRear_Speed < 10)
        Motor_LeftRear_Sp
    if (Motor_RightFront_Speed < 10)eed = 0;
        Motor_RightFront_Speed = 0;
    if (Motor_RightRear_Speed < 10)
        Motor_RightRear_Speed = 0;
}