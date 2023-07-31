#include "servo.h"

Servo Servo_0, Servo_1;

uint8_t Servo_Control_Mode;
uint8_t Servo_UART_Rx_Byte;
uint8_t cmd[50];

void Servo_Init(void)
{
    Servo_0.ID = 0;
    Servo_0.Current_PWM = SERVO_PWM_CENTER;
    Servo_0.Current_Angle = SERVO_ANGLE_CENTER;
    Servo_0.Target_PWM = SERVO_PWM_CENTER;
    Servo_0.Target_Angle = SERVO_ANGLE_CENTER;
    
    Servo_1.ID = 1;
    Servo_1.Current_PWM = SERVO_PWM_CENTER;
    Servo_1.Current_Angle = SERVO_ANGLE_CENTER;
    Servo_1.Target_PWM = SERVO_PWM_CENTER;
    Servo_1.Target_Angle = SERVO_ANGLE_CENTER;
}

void Servo_Set_Control_Mode(uint8_t Mode)
{
    if (Mode == SERVO_CONTROL_BY_PWM)
    {
        Servo_Control_Mode = SERVO_CONTROL_BY_PWM;
    }
    else if (Mode == SERVO_CONTROL_BY_ANGLE)
    {
        Servo_Control_Mode = SERVO_CONTROL_BY_ANGLE;
    }
}

void Servo_Set_PWM(uint8_t ServoID, uint16_t PWM)
{
    sprintf((char*)cmd, "#%03dP%04dT0500!", ServoID, PWM < SERVO_PWM_MIN ? SERVO_PWM_MIN : (PWM > SERVO_PWM_MAX ? SERVO_PWM_MAX : PWM));
    HAL_UART_Transmit_DMA(&SERVO_UART_HANDLER, cmd, 15);
    
}

void Servo_Set_Angle(uint8_t ServoID, float Angle)
{
    uint16_t PWM = (uint16_t)(Angle * (SERVO_PWM_MAX - SERVO_PWM_MIN) / (SERVO_ANGLE_MAX - SERVO_ANGLE_MIN) + SERVO_PWM_CENTER);
    Servo_Set_PWM(ServoID, PWM);
}

void Servo_Set_PWM_Group(uint8_t ServoID_1, uint16_t PWM_1, uint8_t ServoID_2, uint16_t PWM_2)
{
    sprintf((char*)cmd, "{G0000#%03dP%04dT0500!#%03dP%04dT0500!}", ServoID_1, PWM_1 < SERVO_PWM_MIN ? SERVO_PWM_MIN : (PWM_1 > SERVO_PWM_MAX ? SERVO_PWM_MAX : PWM_1), ServoID_2, PWM_2 < SERVO_PWM_MIN ? SERVO_PWM_MIN : (PWM_2 > SERVO_PWM_MAX ? SERVO_PWM_MAX : PWM_2));
    HAL_UART_Transmit_DMA(&SERVO_UART_HANDLER, cmd, 37);
}

void Servo_Set_Angle_Group(uint8_t ServoID_1, float Angle_1, uint8_t ServoID_2, float Angle_2)
{
    uint16_t PWM_1 = (uint16_t)(Angle_1 * (SERVO_PWM_MAX - SERVO_PWM_MIN) / (SERVO_ANGLE_MAX - SERVO_ANGLE_MIN) + SERVO_PWM_CENTER);
    uint16_t PWM_2 = (uint16_t)(Angle_2 * (SERVO_PWM_MAX - SERVO_PWM_MIN) / (SERVO_ANGLE_MAX - SERVO_ANGLE_MIN) + SERVO_PWM_CENTER);
    Servo_Set_PWM_Group(ServoID_1, PWM_1, ServoID_2, PWM_2);
}

void Servo_Get_Position(uint8_t ServoID)
{
    sprintf((char*)cmd, "#%03dPRAD!", ServoID);
    HAL_UART_Transmit_IT(&SERVO_UART_HANDLER, cmd, 9);
}

uint16_t Get_PWM_From_Response(uint8_t *Response)
{
    char number[4] = "";
    memcpy(number, 5+(char *)Response, 4);

    // 将字符串转换为整数
    uint16_t pwm = atoi(number);

    return pwm;
}
