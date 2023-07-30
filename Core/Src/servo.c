#include "servo.h"

void Servo_Set_PWM(uint8_t ServoID, uint16_t PWM)
{
    uint8_t cmd[15];
    sprintf((char*)cmd, "#%03dP%04dT0500!", ServoID, PWM);
    HAL_UART_Transmit(&SERVO_UART_HANDLER, cmd, 15, 0xFFFF);
}
