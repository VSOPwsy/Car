#ifndef __SYSTEM_H
#define __SYSTEM_H

#include "gpio.h"
#include "tim.h"
#include "usart.h"
#include "pid.h"
#include "string.h"
#include "stdlib.h"

#define SMOOTH_CONTROL      1

#define MOTOR_PID_KP        				5000.0f
#define MOTOR_PID_KI        				0.0f
#define MOTOR_PID_KD        				0.0f
#define MOTOR_PID_INTEGRAL_LIMIT		2e4f
#define MOTOR_PWM_LIMIT      			63000.0f
#define MOTOR_SPEED_LIMIT   1.0f

#define PERIOD_INTERRUPT_TIM_HANDLER                htim1

#define LEFTFRONT_MOTOR_IN1_GPIO_PORT               GPIOB
#define LEFTFRONT_MOTOR_IN1_GPIO_PIN                GPIO_PIN_8
#define LEFTFRONT_MOTOR_IN2_GPIO_PORT               GPIOB
#define LEFTFRONT_MOTOR_IN2_GPIO_PIN                GPIO_PIN_9
#define LEFTFRONT_MOTOR_PWM_TIM_HANDLER             htim2
#define LEFTFRONT_MOTOR_PWM_TIM_CHANNEL             TIM_CHANNEL_2
#define LEFTFRONT_MOTOR_ENCODER_TIM_HANDLER         htim4

#define LEFTREAR_MOTOR_IN1_GPIO_PORT                GPIOC
#define LEFTREAR_MOTOR_IN1_GPIO_PIN                 GPIO_PIN_2
#define LEFTREAR_MOTOR_IN2_GPIO_PORT                GPIOC
#define LEFTREAR_MOTOR_IN2_GPIO_PIN                 GPIO_PIN_3
#define LEFTREAR_MOTOR_PWM_TIM_HANDLER              htim2
#define LEFTREAR_MOTOR_PWM_TIM_CHANNEL              TIM_CHANNEL_4
#define LEFTREAR_MOTOR_ENCODER_TIM_HANDLER          htim3

#define RIGHTFRONT_MOTOR_IN1_GPIO_PORT              GPIOB
#define RIGHTFRONT_MOTOR_IN1_GPIO_PIN               GPIO_PIN_4
#define RIGHTFRONT_MOTOR_IN2_GPIO_PORT              GPIOB
#define RIGHTFRONT_MOTOR_IN2_GPIO_PIN               GPIO_PIN_5
#define RIGHTFRONT_MOTOR_PWM_TIM_HANDLER            htim2
#define RIGHTFRONT_MOTOR_PWM_CHANNEL                TIM_CHANNEL_1
#define RIGHTFRONT_MOTOR_ENCODER_TIM_HANDLER        htim8

#define RIGHTREAR_MOTOR_IN1_GPIO_PORT               GPIOC
#define RIGHTREAR_MOTOR_IN1_GPIO_PIN                GPIO_PIN_0
#define RIGHTREAR_MOTOR_IN2_GPIO_PORT               GPIOC
#define RIGHTREAR_MOTOR_IN2_GPIO_PIN                GPIO_PIN_1
#define RIGHTREAR_MOTOR_PWM_TIM_HANDLER             htim2
#define RIGHTREAR_MOTOR_PWM_CHANNEL                 TIM_CHANNEL_3
#define RIGHTREAR_MOTOR_ENCODER_TIM_HANDLER         htim5

#define WHEEL_DIAMETER              0.065f
#define ENCODER_MULTIPLES           4
#define ENCODER_PRECISION           (ENCODER_MULTIPLES*364)
#define WHEEL_DISTANCE              0.102f
#define AXLE_DISTANCE               0.170f

#define PI						  	3.141592f

#define SERVO_UART_HANDLER          huart3

#define TRACK_TASK_UART_HANDLER     huart2



extern float Move_X, Move_Y, Move_Z;

typedef struct
{
	float Encoder;
	float Motor_PWM;
	float Target;
}Motor_Parameter;
extern Motor_Parameter Motor_LeftFront, Motor_LeftRear, Motor_RightFront, Motor_RightRear;


typedef struct
{
	float VX;
	float VY;
	float VZ;
}Smooth_Control;
extern Smooth_Control smooth_control;


typedef union {
    float as_float;
    uint8_t as_bytes[4];
} float_union;


typedef union {
    uint16_t as_uint16;
    uint8_t as_bytes[2];
} uint16_union;


float target_limit_float(float, float, float);
int target_limit_int(int, int, int);
float float_abs(float);

#endif
