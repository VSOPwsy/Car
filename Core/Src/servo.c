#include "servo.h"

uint8_t Servo_UART_Rx_Byte;
uint16_t Servo_0_Current_PWM;
uint16_t Servo_1_Current_PWM;

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

void Servo_Get_PWM(uint8_t ServoID)
{
    uint8_t cmd[9];
    sprintf((char*)cmd, "#%03dPRAD!", ServoID);
    HAL_UART_Transmit_IT(&SERVO_UART_HANDLER, cmd, 9);
}

void Servo_Get_Angle(uint8_t ServoID)
{
    uint16_t PWM = Servo_Get_PWM(ServoID);
    return (float)(PWM - SERVO_PWM_CENTER) * (SERVO_ANGLE_MAX - SERVO_ANGLE_MIN) / (SERVO_PWM_MAX - SERVO_PWM_MIN);
}


void Servo_Response_UART_Rx_Byte()
{
    if (Servo_Response_UART_Rx.Index == 0 && Servo_UART_Rx_Byte == SERVO_COMMAND_HEADER)
    {
        Servo_Response_UART_Rx.Response_Temp[0] = Servo_UART_Rx_Byte;
        Servo_Response_UART_Rx.Index++;
    }
    else if (Servo_Response_UART_Rx.Index > 0 && Servo_Response_UART_Rx.Index < 9)
    {
        Servo_Response_UART_Rx.Response_Temp[Servo_Response_UART_Rx.Index] = Servo_UART_Rx_Byte;
        Servo_Response_UART_Rx.Index++;
    }
    else if (Servo_Response_UART_Rx.Index == 9 && Servo_UART_Rx_Byte == SERVO_COMMAND_TAIL)
    {
        Servo_Response_UART_Rx.Response_Temp[9] = Servo_UART_Rx_Byte;
        Servo_Response_UART_Rx.Index = 0;
        memcpy(Servo_Response_UART_Rx.Response, Servo_Response_UART_Rx.Response_Temp, 10);
        Servo_Response_UART_Rx.Flag = 1;

        char id_number[3] = "", pwm_number[4] = "";
        strncpy(id_number, 1, 3);
        strncpy(pwm_number, 4, 7);
        uint8_t id = atoi(id_number), pwm = atoi(pwm_number);

        if (id == 0x00)
        {
            Servo_0_Current_PWM = pwm;
        }
        else if (id == 0x01)
        {
            Servo_1_Current_PWM = pwm;
        }
    }
    else
    {
        Servo_Response_UART_Rx.Index = 0;
    }
}

