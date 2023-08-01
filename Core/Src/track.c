#include "track.h"

Coor_UART_Rx Coordinates_UART_Rx;
uint8_t Track_UART_Rx_Byte;

void Track_Init(void)
{
    Coordinates_UART_Rx.Header = 0xAA;
    Coordinates_UART_Rx.Tail = 0x55;
		Coordinates_UART_Rx.Coordinates.Y.as_uint16 = 120;
		Coordinates_UART_Rx.Coordinates.X.as_uint16 = 160;
}

void Determine_Angle(void){

    if(Coordinates_UART_Rx.Coordinates.Y.as_uint16>200){          //如果y坐标偏离较大，则向上或向下转动
        Servo_0.Target_Angle=60+Servo_0.Current_Angle;
    }else if(Coordinates_UART_Rx.Coordinates.Y.as_uint16>150){
        Servo_0.Target_Angle=30+Servo_0.Current_Angle;
    }else if(Coordinates_UART_Rx.Coordinates.Y.as_uint16<90){
        Servo_0.Target_Angle=-60+Servo_0.Current_Angle;
    }else if(Coordinates_UART_Rx.Coordinates.Y.as_uint16<40){
        Servo_0.Target_Angle=-30+Servo_0.Current_Angle;
    }else{
			Servo_0.Target_Angle = Servo_0.Current_Angle;
    }

    if(Coordinates_UART_Rx.Coordinates.X.as_uint16>280){          //如果x坐标偏离较大,则向左或向右转动
        Servo_1.Target_Angle=-60+Servo_1.Current_Angle;
    }else if(Coordinates_UART_Rx.Coordinates.X.as_uint16>200){
        Servo_1.Target_Angle=-30+Servo_1.Current_Angle;
    }else if(Coordinates_UART_Rx.Coordinates.X.as_uint16<40){
        Servo_1.Target_Angle=60+Servo_1.Current_Angle;
    }else if(Coordinates_UART_Rx.Coordinates.X.as_uint16<120){
        Servo_1.Target_Angle=30+Servo_1.Current_Angle;
    }else{
			Servo_1.Target_Angle = Servo_1.Current_Angle;
    }

  
    
}

void Coordinates_UART_Rx_Byte()
{
    switch (Coordinates_UART_Rx.Rx_Index)
    {
    case 0:
      if (Track_UART_Rx_Byte == Coordinates_UART_Rx.Header)
      {
        Coordinates_UART_Rx.Rx_Index++;
      }      
      break;

    case 1:
        Coordinates_UART_Rx.Coordinates_Buffer.X.as_bytes[1] = Track_UART_Rx_Byte;
        Coordinates_UART_Rx.Rx_Index++;
        break;

    case 2:
        Coordinates_UART_Rx.Coordinates_Buffer.X.as_bytes[0] = Track_UART_Rx_Byte;
        Coordinates_UART_Rx.Rx_Index++;
        break;

    case 3:
        Coordinates_UART_Rx.Coordinates_Buffer.Y.as_bytes[1] = Track_UART_Rx_Byte;
        Coordinates_UART_Rx.Rx_Index++;
        break;

    case 4:
        Coordinates_UART_Rx.Coordinates_Buffer.Y.as_bytes[0] = Track_UART_Rx_Byte;
        Coordinates_UART_Rx.Rx_Index++;
        break;

    case 5:
        if (Track_UART_Rx_Byte == Coordinates_UART_Rx.Tail)
        {
            Coordinates_UART_Rx.Rx_Index = 0;

            Coordinates_UART_Rx.Coordinates.X.as_uint16 = Coordinates_UART_Rx.Coordinates_Buffer.X.as_uint16;
						Coordinates_UART_Rx.Coordinates.Y.as_uint16 = Coordinates_UART_Rx.Coordinates_Buffer.Y.as_uint16;
        }
        else
        {
            Coordinates_UART_Rx.Rx_Index = 0;
        }
        break;
    
    default:
        Coordinates_UART_Rx.Rx_Index = 0;
      break;
    }
}
