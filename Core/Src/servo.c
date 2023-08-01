#include "servo.h"

Servo Servo_0, Servo_1;

uint8_t Servo_Control_Mode;
uint8_t Servo_UART_Rx_Byte;
uint8_t cmd[50];

void Servo_Init(void)
{
    Servo_0.ID = 0;
    Servo_0.Angle_Max = (SERVO_0_PWM_MAX - 500) * 270.0f / 2000 - 135;
    Servo_0.Angle_Min = (SERVO_0_PWM_MIN - 500) * 270.0f / 2000 - 135;
    Servo_0.PWM_Max = SERVO_0_PWM_MAX;
    Servo_0.PWM_Min = SERVO_0_PWM_MIN;
    Servo_0.Target_PWM = (Servo_0.PWM_Max + Servo_0.PWM_Min) / 2;
    Servo_0.Target_Angle = (Servo_0.Angle_Max + Servo_0.Angle_Min) / 2;
    Servo_0.Current_Angle = Servo_0.Target_Angle;
    Servo_0.Current_PWM = Servo_0.Target_PWM;
    
    Servo_1.ID = 1;
    Servo_1.Angle_Max = (SERVO_1_PWM_MAX - 500) * 270.0f / 2000 - 135;
    Servo_1.Angle_Min = (SERVO_1_PWM_MIN - 500) * 270.0f / 2000 - 135;
    Servo_1.PWM_Max = SERVO_1_PWM_MAX;
    Servo_1.PWM_Min = SERVO_1_PWM_MIN;
    Servo_1.Target_PWM = (Servo_1.PWM_Max + Servo_1.PWM_Min) / 2;
    Servo_1.Target_Angle = (Servo_1.Angle_Max + Servo_1.Angle_Min) / 2;
    Servo_1.Current_Angle = Servo_1.Target_Angle;
    Servo_1.Current_PWM = Servo_1.Target_PWM;
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

void Servo_Set_PWM(Servo servo, uint16_t PWM)
{
    sprintf((char*)cmd, "#%03dP%04dT0500!", servo.ID, PWM < servo.PWM_Min ? servo.PWM_Min : (PWM > servo.PWM_Max ? servo.PWM_Max : PWM));
    HAL_UART_Transmit_DMA(&SERVO_UART_HANDLER, cmd, 15);
    
}

void Servo_Set_Angle(Servo servo, float Angle)
{
    float agl = Angle < servo.Angle_Min ? servo.Angle_Min : (Angle > servo.Angle_Max ? servo.Angle_Max : Angle);
    uint16_t PWM = (agl+135)*(servo.PWM_Max - servo.PWM_Min)/(servo.Angle_Max - servo.Angle_Min) + 500;
    Servo_Set_PWM(servo, PWM);
}

void Servo_Get_Position(Servo servo)
{
    sprintf((char*)cmd, "#%03dPRAD!", servo.ID);
    HAL_UART_Transmit_IT(&SERVO_UART_HANDLER, cmd, 9);
}

void Servo_Stop(Servo servo)
{
    sprintf((char*)cmd, "#%03dPDST!", servo.ID);
    HAL_UART_Transmit_IT(&SERVO_UART_HANDLER, cmd, 9);
}

uint16_t Get_PWM_From_Response(uint8_t *Response)
{
    char number[4] = "";
    strncpy(number, 5+(char *)Response, 4);

    // 将字符串转换为整数
    uint16_t pwm = atoi(number);

    return pwm;
}
