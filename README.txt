PWM(TIM2):
LeftFront : PB8, PB9, PB3 (CH2)  ->  Front TB6612: BIN1, BIN2, PWMB
LeftRear  : PC2, PC3, PA3 (CH4)  ->  Rear  TB6612: AIN1, AIN2, PWMA
RightFront: PB4, PB5, PA15(CH1)  ->  Front TB6612: AIN1, AIN2, PWMA
RightRear : PC0, PC1, PA2 (CH3)  ->  Rear  TB6612: BIN1, BIN2, PWMB

Encoder:
LeftFront : PB6(TIM4_CH1), PB7(TIM4_CH2)  ->  Front TB6612: E2A, E2B
LeftRear  : PA6(TIM3_CH1), PA7(TIM3_CH2)  ->  Rear  TB6612: E1A, E1B
RighFront : PC6(TIM8_CH1), PC7(TIM8_CH2)  ->  Front TB6612: E1A, E1B
RightRear : PA0(TIM5_CH1), PA1(TIM5_CH2)  ->  Rear  TB6612: E2A, E2B


Servo(USART3):
Servo#000(upper):
            min         center          max
    PWM:    834         1500            2166
    Angle:  -90         0               90

Servo#001(lower):
            min         center          max
    PWM:    1500        2000            2500
    Angle:  0           67.5            135

last version:   9aa106a    7/31
