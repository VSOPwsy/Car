#ifndef __SERVO_H
#define __SERVO_H

#include "system.h"
#include "usart.h"


#define SERVO_PWM_MIN 500
#define SERVO_PWM_MAX 2500
#define SERVO_PWM_CENTER 1500

#define SERVO_ANGLE_MIN -135.0f
#define SERVO_ANGLE_MAX 135.0f
#define SERVO_ANGLE_CENTER 0.0f

void Servo_Set_PWM(uint8_t ServoID, uint16_t PWM);  // from 500 to 2500
void Servo_Set_Angle(uint8_t ServoID, float Angle); // from -135 to 135

uint16_t Servo_Get_PWM(uint8_t ServoID);
float Servo_Get_Angle(uint8_t ServoID);

#endif
