#include "motor.h"

PID PID_Motor_LeftFront;
PID PID_Motor_RightFront;
PID PID_Motor_LeftRear;
PID PID_Motor_RightRear;

void Motor_Init(void)
{
    HAL_TIM_PWM_Start(&LEFTFRONT_MOTOR_PWM_TIM_HANDLER,  LEFTFRONT_MOTOR_PWM_TIM_CHANNEL);
    HAL_TIM_PWM_Start(&RIGHTFRONT_MOTOR_PWM_TIM_HANDLER, RIGHTFRONT_MOTOR_PWM_CHANNEL);
    HAL_TIM_PWM_Start(&LEFTREAR_MOTOR_PWM_TIM_HANDLER,   LEFTREAR_MOTOR_PWM_TIM_CHANNEL);
    HAL_TIM_PWM_Start(&RIGHTREAR_MOTOR_PWM_TIM_HANDLER,  RIGHTREAR_MOTOR_PWM_CHANNEL);

    HAL_TIM_Encoder_Start(&LEFTFRONT_MOTOR_ENCODER_TIM_HANDLER, TIM_CHANNEL_ALL);
    HAL_TIM_Encoder_Start(&RIGHTFRONT_MOTOR_ENCODER_TIM_HANDLER, TIM_CHANNEL_ALL);
    HAL_TIM_Encoder_Start(&LEFTREAR_MOTOR_ENCODER_TIM_HANDLER, TIM_CHANNEL_ALL);
    HAL_TIM_Encoder_Start(&RIGHTREAR_MOTOR_ENCODER_TIM_HANDLER, TIM_CHANNEL_ALL);

    PID_Init(&PID_Motor_LeftFront,  MOTOR_PID_KP, 0, MOTOR_PID_KD);
    PID_Init(&PID_Motor_RightFront, MOTOR_PID_KP, 0, MOTOR_PID_KD);
    PID_Init(&PID_Motor_LeftRear,   MOTOR_PID_KP, 0, MOTOR_PID_KD);
    PID_Init(&PID_Motor_RightRear,  MOTOR_PID_KP, 0, MOTOR_PID_KD);

    Move_X = 0;
    Move_Y = 0;
    Move_Z = 0;
}

void Smoothing(float Vx, float Vy, float Vz)
{
    float step=0.05;

	if	   (Vx>0) 	smooth_control.VX += step;
	else if(Vx<0)	smooth_control.VX -= step;
	else if(Vx==0)	smooth_control.VX =  smooth_control.VX*0.9f;
	
	if	   (Vy>0)   smooth_control.VY += step;
	else if(Vy<0)	smooth_control.VY -= step;
	else if(Vy==0)	smooth_control.VY = smooth_control.VY*0.9f;
	
	if	   (Vz>0) 	smooth_control.VZ += step;
	else if(Vz<0)	smooth_control.VZ -= step;
	else if(Vz==0)	smooth_control.VZ =  smooth_control.VZ*0.9f;
	
	smooth_control.VX = target_limit_float(smooth_control.VX, -float_abs(Vx), float_abs(Vx));
	smooth_control.VY = target_limit_float(smooth_control.VY, -float_abs(Vy), float_abs(Vy));
	smooth_control.VZ = target_limit_float(smooth_control.VZ, -float_abs(Vz), float_abs(Vz));
}

void Measure_Motor_Speed(void)
{
    int32_t threshold = 60000;

    int32_t LeftFront_cnt  = __HAL_TIM_GET_COUNTER(&LEFTFRONT_MOTOR_ENCODER_TIM_HANDLER);
    int32_t RightFront_cnt = __HAL_TIM_GET_COUNTER(&RIGHTFRONT_MOTOR_ENCODER_TIM_HANDLER);
    int32_t LeftRear_cnt   = __HAL_TIM_GET_COUNTER(&LEFTREAR_MOTOR_ENCODER_TIM_HANDLER);
    int32_t RightRear_cnt  = __HAL_TIM_GET_COUNTER(&RIGHTREAR_MOTOR_ENCODER_TIM_HANDLER);

	LeftFront_cnt  = LeftFront_cnt < threshold ? LeftFront_cnt : LeftFront_cnt - 65535;
    LeftFront_cnt  = LeftFront_cnt >-threshold ? LeftFront_cnt : LeftFront_cnt + 65535;
    RightFront_cnt = RightFront_cnt < threshold ? RightFront_cnt : RightFront_cnt - 65535;
    RightFront_cnt = RightFront_cnt >-threshold ? RightFront_cnt : RightFront_cnt + 65535;
    LeftRear_cnt   = LeftRear_cnt < threshold ? LeftRear_cnt : LeftRear_cnt - 65535;
    LeftRear_cnt   = LeftRear_cnt >-threshold ? LeftRear_cnt : LeftRear_cnt + 65535;
    RightRear_cnt  = RightRear_cnt < threshold ? RightRear_cnt : RightRear_cnt - 65535;
    RightRear_cnt  = RightRear_cnt >-threshold ? RightRear_cnt : RightRear_cnt + 65535;
    
    // divide by 10ms, that is, times 100
    Motor_LeftFront.Encoder  =  LeftFront_cnt  *100*PI*WHEEL_DIAMETER/ENCODER_PRECISION;
    Motor_RightFront.Encoder =  RightFront_cnt *100*PI*WHEEL_DIAMETER/ENCODER_PRECISION;
    Motor_LeftRear.Encoder   =  LeftRear_cnt   *100*PI*WHEEL_DIAMETER/ENCODER_PRECISION;
    Motor_RightRear.Encoder  =  RightRear_cnt  *100*PI*WHEEL_DIAMETER/ENCODER_PRECISION;
	
    __HAL_TIM_SET_COUNTER(&LEFTFRONT_MOTOR_ENCODER_TIM_HANDLER, 0);
    __HAL_TIM_SET_COUNTER(&RIGHTFRONT_MOTOR_ENCODER_TIM_HANDLER, 0);
    __HAL_TIM_SET_COUNTER(&LEFTREAR_MOTOR_ENCODER_TIM_HANDLER, 0);
    __HAL_TIM_SET_COUNTER(&RIGHTREAR_MOTOR_ENCODER_TIM_HANDLER, 0);
		
		
}

void Solve_Speed(float Vx, float Vy, float Vz)
{
    if (SMOOTH_CONTROL != 0)
    {
        Smoothing(Vx, Vy, Vz);
        Vx = smooth_control.VX;
        Vy = smooth_control.VY;
        Vz = smooth_control.VZ;
    }
    
    Motor_LeftFront.Target  = +Vy+Vx-Vz*(AXLE_DISTANCE + WHEEL_DISTANCE);
    Motor_RightFront.Target = -Vy+Vx-Vz*(AXLE_DISTANCE + WHEEL_DISTANCE);
    Motor_LeftRear.Target   = +Vy+Vx+Vz*(AXLE_DISTANCE + WHEEL_DISTANCE);
    Motor_RightRear.Target  = -Vy+Vx+Vz*(AXLE_DISTANCE + WHEEL_DISTANCE);

    Motor_LeftFront.Target  = target_limit_float(Motor_LeftFront.Target,  -MOTOR_SPEED_LIMIT , MOTOR_SPEED_LIMIT);
    Motor_RightFront.Target = target_limit_float(Motor_RightFront.Target, -MOTOR_SPEED_LIMIT , MOTOR_SPEED_LIMIT);
    Motor_LeftRear.Target   = target_limit_float(Motor_LeftRear.Target,   -MOTOR_SPEED_LIMIT , MOTOR_SPEED_LIMIT);
    Motor_RightRear.Target  = target_limit_float(Motor_RightRear.Target,  -MOTOR_SPEED_LIMIT , MOTOR_SPEED_LIMIT);
}


void Motor_Set_PWM()
{
    __HAL_TIM_SET_COMPARE(&LEFTFRONT_MOTOR_PWM_TIM_HANDLER,  LEFTFRONT_MOTOR_PWM_TIM_CHANNEL,  float_abs(Motor_LeftFront.Motor_PWM));
    HAL_GPIO_WritePin(LEFTFRONT_MOTOR_IN1_GPIO_PORT, LEFTFRONT_MOTOR_IN1_GPIO_PIN, Motor_LeftFront.Motor_PWM>0?GPIO_PIN_SET:GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LEFTFRONT_MOTOR_IN2_GPIO_PORT, LEFTFRONT_MOTOR_IN2_GPIO_PIN, Motor_LeftFront.Motor_PWM<0?GPIO_PIN_SET:GPIO_PIN_RESET);

//    __HAL_TIM_SET_COMPARE(&RIGHTFRONT_MOTOR_PWM_TIM_HANDLER,  RIGHTFRONT_MOTOR_PWM_CHANNEL,  float_abs(Motor_RightFront.Motor_PWM));
//    HAL_GPIO_WritePin(RIGHTFRONT_MOTOR_IN1_GPIO_PORT, RIGHTFRONT_MOTOR_IN1_GPIO_PIN, Motor_RightFront.Motor_PWM>0?GPIO_PIN_SET:GPIO_PIN_RESET);
//    HAL_GPIO_WritePin(RIGHTFRONT_MOTOR_IN2_GPIO_PORT, RIGHTFRONT_MOTOR_IN2_GPIO_PIN, Motor_RightFront.Motor_PWM<0?GPIO_PIN_SET:GPIO_PIN_RESET);
	
		__HAL_TIM_SET_COMPARE(&RIGHTFRONT_MOTOR_PWM_TIM_HANDLER,  RIGHTFRONT_MOTOR_PWM_CHANNEL,  60000);
    HAL_GPIO_WritePin(RIGHTFRONT_MOTOR_IN1_GPIO_PORT, RIGHTFRONT_MOTOR_IN1_GPIO_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(RIGHTFRONT_MOTOR_IN2_GPIO_PORT, RIGHTFRONT_MOTOR_IN2_GPIO_PIN, GPIO_PIN_RESET);

    __HAL_TIM_SET_COMPARE(&LEFTREAR_MOTOR_PWM_TIM_HANDLER,  LEFTREAR_MOTOR_PWM_TIM_CHANNEL,  float_abs(Motor_LeftRear.Motor_PWM));
    HAL_GPIO_WritePin(LEFTREAR_MOTOR_IN1_GPIO_PORT, LEFTREAR_MOTOR_IN1_GPIO_PIN, Motor_LeftRear.Motor_PWM>0?GPIO_PIN_SET:GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LEFTREAR_MOTOR_IN2_GPIO_PORT, LEFTREAR_MOTOR_IN2_GPIO_PIN, Motor_LeftRear.Motor_PWM<0?GPIO_PIN_SET:GPIO_PIN_RESET);

    __HAL_TIM_SET_COMPARE(&RIGHTREAR_MOTOR_PWM_TIM_HANDLER,  RIGHTREAR_MOTOR_PWM_CHANNEL,  float_abs(Motor_RightRear.Motor_PWM));
    HAL_GPIO_WritePin(RIGHTREAR_MOTOR_IN1_GPIO_PORT, RIGHTREAR_MOTOR_IN1_GPIO_PIN, Motor_RightRear.Motor_PWM>0?GPIO_PIN_SET:GPIO_PIN_RESET);
    HAL_GPIO_WritePin(RIGHTREAR_MOTOR_IN2_GPIO_PORT, RIGHTREAR_MOTOR_IN2_GPIO_PIN, Motor_RightRear.Motor_PWM<0?GPIO_PIN_SET:GPIO_PIN_RESET);

    // printf("PID_target=%f,Motor_Encoder=%f,Motor_PWM=%f\r\n", PID_Motor_LeftFront.target, Motor_LeftFront.Encoder, Motor_LeftFront.Motor_PWM);
}

void Update_Motor_PID()
{
    PID_Motor_LeftFront.target  = Motor_LeftFront.Target;
    PID_Motor_RightFront.target = Motor_RightFront.Target;
    PID_Motor_LeftRear.target   = Motor_LeftRear.Target;
    PID_Motor_RightRear.target  = Motor_RightRear.Target;

    Update_PID_DerivKnown(&PID_Motor_LeftFront,  Motor_LeftFront.Encoder,  0, &(Motor_LeftFront.Motor_PWM));
    Update_PID_DerivKnown(&PID_Motor_RightFront, Motor_RightFront.Encoder, 0, &(Motor_RightFront.Motor_PWM));
    Update_PID_DerivKnown(&PID_Motor_LeftRear,   Motor_LeftRear.Encoder,   0, &(Motor_LeftRear.Motor_PWM));
    Update_PID_DerivKnown(&PID_Motor_RightRear,  Motor_RightRear.Encoder,  0, &(Motor_RightRear.Motor_PWM));

    Motor_LeftFront.Motor_PWM = target_limit_float(Motor_LeftFront.Motor_PWM, -MOTOR_PWM_LIMIT, MOTOR_PWM_LIMIT);
    Motor_RightFront.Motor_PWM = target_limit_float(Motor_RightFront.Motor_PWM, -MOTOR_PWM_LIMIT, MOTOR_PWM_LIMIT);
    Motor_LeftRear.Motor_PWM = target_limit_float(Motor_LeftRear.Motor_PWM, -MOTOR_PWM_LIMIT, MOTOR_PWM_LIMIT);
    Motor_RightRear.Motor_PWM = target_limit_float(Motor_RightRear.Motor_PWM, -MOTOR_PWM_LIMIT, MOTOR_PWM_LIMIT);

}

