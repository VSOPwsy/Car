#include "servo.h"

void Servo_Set_PWM(uint8_t ServoID, uint16_t PWM)
{
    uint8_t cmd[15];
    sprintf((char*)cmd, "#%03dP%04dT0500!", ServoID, PWM < SERVO_PWM_MIN ? SERVO_PWM_MIN : (PWM > SERVO_PWM_MAX ? SERVO_PWM_MAX : PWM));
    HAL_UART_Transmit_IT(&SERVO_UART_HANDLER, cmd, 15);
    
}

void Servo_Set_Angle(uint8_t ServoID, float Angle)
{
    uint16_t PWM = (uint16_t)(Angle * (SERVO_PWM_MAX - SERVO_PWM_MIN) / (SERVO_ANGLE_MAX - SERVO_ANGLE_MIN) + SERVO_PWM_CENTER);
    Servo_Set_PWM(ServoID, PWM);
}

uint16_t Servo_Get_PWM(uint8_t ServoID)
{
    uint8_t cmd[9];
    sprintf((char*)cmd, "#%03dPRAD!", ServoID);
    HAL_UART_Transmit_IT(&SERVO_UART_HANDLER, cmd, 9);
}

float Servo_Get_Angle(uint8_t ServoID)
{
    uint16_t PWM = Servo_Get_PWM(ServoID);
    return (float)(PWM - SERVO_PWM_CENTER) * (SERVO_ANGLE_MAX - SERVO_ANGLE_MIN) / (SERVO_PWM_MAX - SERVO_PWM_MIN);
}

