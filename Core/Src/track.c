#include "track.h"

Coor_UART_Rx Coordinates_UART_Rx;
uint8_t Track_UART_Rx_Byte;

void Track_Init(void)
{
    Coordinates_UART_Rx.Header = 0xAA;
    Coordinates_UART_Rx.Tail = 0x55;
}

void Determine_Angle(void){
    float angle_current_0;
    float angle_current_1;
    angle_current_0 = Servo_Get_Angle(0x00);            //得到当前的角度
    angle_current_1 = Servo_Get_Angle(0x01);

    if(Coordinates_UART_Rx.Coordinates.Y>200){          //如果y坐标偏离较大，则向上或向下转动
        Servo_Set_Angle(0x01,60+angle_current_1);
    }else if(Coordinates_UART_Rx.Coordinates.Y>150){
        Servo_Set_Angle(0x01,30+angle_current_1);
    }else if(Coordinates_UART_Rx.Coordinates.Y<90){
        Servo_Set_Angle(0x01,-30+angle_current_1);
    }else if(Coordinates_UART_Rx.Coordinates.Y<40){
        Servo_Set_Angle(0x01,-60+angle_current_1);
    }else{
        Servo_Set_Angle(0x01,angle_current_1);
    }

    if(Coordinates_UART_Rx.Coordinates.X>260){          //如果x坐标偏离较大，则向左或向右转动
        Servo_Set_Angle(0x00,60+angle_current_0);
    }else if(Coor_UART_Rx.Coordinatex.X>200){
        Servo_Set_Angle(0x00,30+angle_current_0);
    }else if(Coor_UART_Rx.Coordinates.X<60){
        Servo_Set_Angle(0x00,-60+angle_current_0);
    }else if(Coor_UART_Rx.Coordinates.X<120){
        Servo_Set_Angle(0x00,-30+angle_current_0);
    }else{
        Servo_Set_Angle(0x00,angle_current_0);
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
        Coordinates_UART_Rx.Coordinates_Buffer.X.as_bytes[0] = Track_UART_Rx_Byte;
        Coordinates_UART_Rx.Rx_Index++;
        break;

    case 2:
        Coordinates_UART_Rx.Coordinates_Buffer.X.as_bytes[1] = Track_UART_Rx_Byte;
        Coordinates_UART_Rx.Rx_Index++;
        break;

    case 3:
        Coordinates_UART_Rx.Coordinates_Buffer.Y.as_bytes[0] = Track_UART_Rx_Byte;
        Coordinates_UART_Rx.Rx_Index++;
        break;

    case 4:
        Coordinates_UART_Rx.Coordinates_Buffer.Y.as_bytes[1] = Track_UART_Rx_Byte;
        Coordinates_UART_Rx.Rx_Index++;
        break;

    case 5:
        if (Track_UART_Rx_Byte == Coordinates_UART_Rx.Tail)
        {
            Coordinates_UART_Rx.Rx_Index = 0;

            Coordinates_UART_Rx.Coordinates.X.as_uint16 = Coordinates_UART_Rx.Coordinates_Buffer.X.as_uint16;
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
