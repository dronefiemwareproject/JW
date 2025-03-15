#include "uart1.h"

void UART1_Init()
{
	UCSR1A |= 1 << U2X1;
	UBRR1H = 0;
	UBRR1L = BAUD_9600;
	UCSR1B |= 1 << RXEN1 | 1 << RXCIE1;
	UCSR1B |= 1 << TXEN1; // | 1 << TXCIE0;
}

void UART1_Transmit(unsigned char data)
{
	while(!(UCSR1A & (1 << UDRE1)));
	UDR1 = data;
}
unsigned char UART1_Receive()
{
	while(!(UCSR1A & (1 << RXC1)));
	return UDR1;
}