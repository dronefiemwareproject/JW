#include "uart0.h"

void UART0_Init()
{
	UCSR0A |= 1 << U2X0;
	UBRR0H = 0;
	UBRR0L = BAUD_9600;
	UCSR0B |= 1 << RXEN0 | 1 << RXCIE0;
	UCSR0B |= 1 << TXEN0; // | 1 << TXCIE0;
}
void UART0_Transmit(unsigned char data)
{
	while(!(UCSR0A & (1 << UDRE0)));
	UDR0 = data;
}
unsigned char UART0_Receive()
{
	while(!(UCSR0A & (1 << RXC0)));
	return UDR0;
}