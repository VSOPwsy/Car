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


extern uint8_t Servo_UART_Rx_Byte;
extern uint16_t Servo_1_Current_PWM;
extern uint16_t Servo_2_Current_PWM;

typedef struct
{
    uint8_t Index;
    uint8_t Response_Temp[10];
    uint8_t Response[10];
    uint8_t Flag;
} Srv_Response;
extern Srv_Response Servo_Response_UART_Rx;

void Servo_Set_PWM(uint8_t ServoID, uint16_t PWM);  // from 500 to 2500
void Servo_Set_Angle(uint8_t ServoID, float Angle); // from -135 to 135
void Servo_Set_PWM_Group(uint8_t ServoID_1, uint16_t PWM_1, uint8_t ServoID_2, uint16_t PWM_2);
void Servo_Set_Angle_Group(uint8_t ServoID_1, float Angle_1, uint8_t ServoID_2, float Angle_2);

void Servo_Response_UART_Rx_Byte(void);

uint16_t Servo_Get_PWM(uint8_t ServoID);
float Servo_Get_Angle(uint8_t ServoID);

#endif
