#ifndef __SERVO_H
#define __SERVO_H

#include "system.h"
#include "usart.h"

#define SERVO_0_PWM_MIN       834
#define SERVO_0_PWM_MAX       2166

#define SERVO_1_PWM_MIN       1500
#define SERVO_1_PWM_MAX       2166

#define SERVO_COMMAND_HEADER      '#'
#define SERVO_COMMAND_TAIL        '!'

#define SERVO_CONTROL_BY_PWM       0
#define SERVO_CONTROL_BY_ANGLE     1

#define SERVO_0_SET                 0
#define SERVO_0_GET                 1
#define SERVO_1_SET                 2
#define SERVO_1_GET                 3

extern uint8_t Servo_Control_Mode;

extern uint8_t Servo_UART_Rx_Byte;
extern uint8_t cmd[50];

typedef struct
{
    uint8_t Index;
    uint8_t Response_Temp[10];
    uint8_t Response[10];
} Srv_Response;

typedef struct
{
    uint8_t ID;
    uint16_t PWM_Max;
    uint16_t PWM_Min;
    float Angle_Max;
    float Angle_Min;
    uint16_t Current_PWM;
    float Current_Angle;
    uint16_t Target_PWM;
    float Target_Angle;
    Srv_Response Response_UART_Rx;
} Servo;
extern Servo Servo_0, Servo_1;

void Servo_Init(void);

void Servo_Set_Control_Mode(uint8_t Mode);

void Servo_Set_PWM(Servo servo, uint16_t PWM);
void Servo_Set_Angle(Servo servo, float Angle);

void Servo_Get_Position(Servo servo);

void Servo_Stop(Servo servo);

uint16_t Get_PWM_From_Response(uint8_t *Response);

#endif
