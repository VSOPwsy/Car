#ifndef __SERVO_H
#define __SERVO_H

#include "system.h"
#include "usart.h"

#define SERVO_PWM_MIN       500
#define SERVO_PWM_MAX       2500
#define SERVO_PWM_CENTER    1500

#define SERVO_ANGLE_MIN    -135.0f
#define SERVO_ANGLE_MAX     135.0f
#define SERVO_ANGLE_CENTER  0.0f

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
    uint16_t Current_PWM;
    float Current_Angle;
    uint16_t Target_PWM;
    float Target_Angle;
    Srv_Response Response_UART_Rx;
} Servo;
extern Servo Servo_0, Servo_1;

void Servo_Init(void);

void Servo_Set_Control_Mode(uint8_t Mode);

void Servo_Set_PWM(uint8_t ServoID, uint16_t PWM);  // from 500 to 2500
void Servo_Set_Angle(uint8_t ServoID, float Angle); // from -135 to 135
void Servo_Set_PWM_Group(uint8_t ServoID_1, uint16_t PWM_1, uint8_t ServoID_2, uint16_t PWM_2);
void Servo_Set_Angle_Group(uint8_t ServoID_1, float Angle_1, uint8_t ServoID_2, float Angle_2);

void Servo_Get_Position(uint8_t ServoID);

uint16_t Get_PWM_From_Response(uint8_t *Response);

#endif
