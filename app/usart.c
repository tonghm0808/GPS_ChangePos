#include "usart.h"
u16 rx_Point = 0;

void USART1_IRQHandler(void)
{
	CoEnterISR();

	if (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET)
	{
		rx_Buff[rx_Point] = USART_ReceiveData(USART1);
		isr_PostQueueMail(usart_Rx, &rx_Buff[rx_Point]);
		if (rx_Point++ == rx_BuffSize)
		{
			rx_Point = 0;
		}
	}

	CoExitISR();
}

void usart_Putchar(u8 ch)
{
	USART_SendData( USART1, ch );
	while ( USART_GetFlagStatus( USART1, USART_FLAG_TXE ) == RESET );
}
