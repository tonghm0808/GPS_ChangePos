#ifndef  USART_H
#define  USART_H

#include "stm32f10x.h"
#include "CoOS.h"

#define rx_BuffSize 1000

u8 rx_Buff[rx_BuffSize];
OS_EventID usart_Rx;

void *rx_Queue[rx_BuffSize];
void usart_Putchar(u8 ch);

#endif
