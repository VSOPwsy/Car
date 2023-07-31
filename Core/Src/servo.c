#include "servo.h"

uint8_t Servo_UART_Rx_Byte;
uint16_t Servo_0_Current_PWM;
uint16_t Servo_1_Current_PWM;
Srv_Response Servo_Response_UART_Rx;

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

void Servo_Set_PWM_Group(uint8_t ServoID_1, uint16_t PWM_1, uint8_t ServoID_2, uint16_t PWM_2)
{
    uint8_t cmd[37];
    sprintf((char*)cmd, "{G0000#%03dP%04dT0500!#%03dP%04dT0500!}", ServoID_1, PWM_1 < SERVO_PWM_MIN ? SERVO_PWM_MIN : (PWM_1 > SERVO_PWM_MAX ? SERVO_PWM_MAX : PWM_1), ServoID_2, PWM_2 < SERVO_PWM_MIN ? SERVO_PWM_MIN : (PWM_2 > SERVO_PWM_MAX ? SERVO_PWM_MAX : PWM_2));
    HAL_UART_Transmit_IT(&SERVO_UART_HANDLER, cmd, 37);
}

void Servo_Set_Angle_Group(uint8_t ServoID_1, float Angle_1, uint8_t ServoID_2, float Angle_2)
{
    uint16_t PWM_1 = (uint16_t)(Angle_1 * (SERVO_PWM_MAX - SERVO_PWM_MIN) / (SERVO_ANGLE_MAX - SERVO_ANGLE_MIN) + SERVO_PWM_CENTER);
    uint16_t PWM_2 = (uint16_t)(Angle_2 * (SERVO_PWM_MAX - SERVO_PWM_MIN) / (SERVO_ANGLE_MAX - SERVO_ANGLE_MIN) + SERVO_PWM_CENTER);
    Servo_Set_PWM_Group(ServoID_1, PWM_1, ServoID_2, PWM_2);
}

void Servo_Get_Position(uint8_t ServoID)
{
    Servo_Response_UART_Rx.Flag = 0;
    uint8_t cmd[9];
    sprintf((char*)cmd, "#%03dPRAD!", ServoID);
    HAL_UART_Transmit_IT(&SERVO_UART_HANDLER, cmd, 9);
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
        strncpy(id_number, (char *)(&Servo_Response_UART_Rx.Response[0] + 1), 3);
        strncpy(pwm_number, (char *)(&Servo_Response_UART_Rx.Response[0] + 4), 7);
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

