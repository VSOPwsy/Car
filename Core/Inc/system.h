#ifndef __SYSTEM_H
#define __SYSTEM_H

#include "gpio.h"

#define LEFTFRONT_MOTOR_IN1_GPIO_PORT GPIOB
#define LEFTFRONT_MOTOR_IN1_GPIO_PIN GPIO_PIN_8
#define LEFTFRONT_MOTOR_IN2_GPIO_PORT GPIOB
#define LEFTFRONT_MOTOR_IN2_GPIO_PIN GPIO_PIN_9
#define LEFTFRONT_MOTOR_PWM_GPIO_PORT GPIOB
#define LEFTFRONT_MOTOR_PWM_GPIO_PIN GPIO_PIN_3

#define LEFTREAR_MOTOR_IN1_GPIO_PORT GPIOC
#define LEFTREAR_MOTOR_IN1_GPIO_PIN GPIO_PIN_0
#define LEFTREAR_MOTOR_IN2_GPIO_PORT GPIOC
#define LEFTREAR_MOTOR_IN2_GPIO_PIN GPIO_PIN_1
#define LEFTREAR_MOTOR_PWM_GPIO_PORT GPIOA
#define LEFTREAR_MOTOR_PWM_GPIO_PIN GPIO_PIN_2

#define RIGHTFRONT_MOTOR_IN1_GPIO_PORT GPIOB
#define RIGHTFRONT_MOTOR_IN1_GPIO_PIN GPIO_PIN_4
#define RIGHTFRONT_MOTOR_IN2_GPIO_PORT GPIOB
#define RIGHTFRONT_MOTOR_IN2_GPIO_PIN GPIO_PIN_5
#define RIGHTFRONT_MOTOR_PWM_GPIO_PORT GPIOA
#define RIGHTFRONT_MOTOR_PWM_GPIO_PIN GPIO_PIN_15

#define RIGHTREAR_MOTOR_IN1_GPIO_PORT GPIOC
#define RIGHTREAR_MOTOR_IN1_GPIO_PIN GPIO_PIN_2
#define RIGHTREAR_MOTOR_IN2_GPIO_PORT GPIOC
#define RIGHTREAR_MOTOR_IN2_GPIO_PIN GPIO_PIN_3
#define RIGHTREAR_MOTOR_PWM_GPIO_PORT GPIOA
#define RIGHTREAR_MOTOR_PWM_GPIO_PIN GPIO_PIN_3

#define WHEEL_DIAMETER 0.065f


extern uint32_t Move_X, Move_Y, Move_Z;


extern uint32_t Motor_LeftFront_Target_Speed;
extern uint32_t Motor_LeftRear_Target_Speed;
extern uint32_t Motor_RightFront_Target_Speed;
extern uint32_t Motor_RightRear_Target_Speed;

extern uint32_t Motor_LeftFront_Speed_Previouse;
extern uint32_t Motor_LeftRear_Speed_Previouse;
extern uint32_t Motor_RightFront_Speed_Previouse;
extern uint32_t Motor_RightRear_Speed_Previouse;



typedef struct  
{
	float Encoder;
	float Motor_PWM;
	float Target;
	float KP;
	float KI;
}Motor_Parameter;


extern Motor_Parameter Motor_LeftFront, Motor_LeftRear, Motor_RightFront, Motor_RightRear;


typedef struct  
{
	float VX;
	float VY;
	float VZ;
}Smooth_Control;

#endif