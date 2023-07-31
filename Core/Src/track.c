#include "track.h"

Coor_UART_Rx Coordinates_UART_Rx;
uint8_t Track_UART_Rx_Byte;

void Track_Init(void)
{
    Coordinates_UART_Rx.Header = 0xAA;
    Coordinates_UART_Rx.Tail = 0x55;
}

void Coordinates_UART_Rx_Byte(uint8_t byte)
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
        Coordinates_UART_Rx.Coordinates_Buffer.X.as_bytes[0] = byte;
        Coordinates_UART_Rx.Rx_Index++;
        break;

    case 2:
        Coordinates_UART_Rx.Coordinates_Buffer.X.as_bytes[1] = byte;
        Coordinates_UART_Rx.Rx_Index++;
        break;

    case 3:
        Coordinates_UART_Rx.Coordinates_Buffer.Y.as_bytes[0] = byte;
        Coordinates_UART_Rx.Rx_Index++;
        break;

    case 4:
        Coordinates_UART_Rx.Coordinates_Buffer.Y.as_bytes[1] = byte;
        Coordinates_UART_Rx.Rx_Index++;
        break;

    case 5:
        if (Track_UART_Rx_Byte == Coordinates_UART_Rx.Tail)
        {
            Coordinates_UART_Rx.Rx_Index = 0;

            Coordinates_UART_Rx.Coordinates.X.as_uint16 = Coordinates_UART_Rx.Coordinates_Buffer.X.as_uint16;
        }
        break;
    
    default:
        Coordinates_UART_Rx.Rx_Index = 0;
      break;
    }
}
