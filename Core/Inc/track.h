#ifndef __TRACK_H
#define __TRACK_H


#include "system.h"


typedef struct
{
    uint16_union X;
    uint16_union Y;
} Coordinates;

typedef struct
{
    uint8_t Rx_Index;
    uint8_t Header;
    uint8_t Tail;
    Coordinates Coordinates_Buffer;
    Coordinates Coordinates;
} Coor_UART_Rx;
extern Coor_UART_Rx Coordinates_UART_Rx;

void Track_Init(void);
void Coordinates_UART_Rx_Byte(uint8_t byte);

extern uint8_t Track_UART_Rx_Byte;



#endif
