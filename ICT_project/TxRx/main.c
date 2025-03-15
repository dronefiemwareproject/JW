// 마스터 - 슬레이브 기본 송수신 코드
#include "main.h"
FILE OUTPUT0 = FDEV_SETUP_STREAM(UART0_Transmit, NULL, _FDEV_SETUP_WRITE);
FILE OUTPUT1 = FDEV_SETUP_STREAM(UART1_Transmit, NULL, _FDEV_SETUP_WRITE);

volatile uint8_t rx0Flag = 0;
volatile uint8_t rx1Flag = 0;
unsigned char str1[100]; // pc -> atmega 데이터가 담기는 배열
unsigned char str2[100]; // 블루투스 -> atmega 데이터가 담기는 배열


int main(void)
{
	UART0_Init();
	UART1_Init();
	DDRD |= 1 << PD3;
	DDRE |= 1 << PE1;
	sei();
	
	while (1)
	{
		// pc -> atmega
		if (rx0Flag == 1)
		{
			rx0Flag = 0;
			stdout = &OUTPUT1; // uart1에 송신 (atmega -> 블루투스)
			printf("%s\n", str1); // pc -> atmega 의 데이터 출력
		}
		
		// 블루투스 -> atmega
		if (rx1Flag == 1)
		{
			rx1Flag = 0;
			stdout = &OUTPUT0; // uart0에 송신 (atmega -> pc)
			printf("%s\n", str2); // 블루투스 -> atmega
		}
	}
}

ISR(USART0_RX_vect) // pc -> atmega로 수신되면
{
	static uint8_t rx0Head = 0;
	uint8_t rx0Data = UDR0;
	if(rx0Data == '\n' || rx0Data == '\r')
	{
		str1[rx0Head] = '\0';
		rx0Head = 0;
		rx0Flag = 1; // pc -> atmega로 수신이 완료됨을 의미
	}
	else
	{
		str1[rx0Head++] = rx0Data;
	}
}ISR(USART1_RX_vect) // 블루투스 -> atmega로 수신되면
{
	static uint8_t rx1Head = 0;
	uint8_t rx1Data = UDR1;
	if(rx1Data == '\n' || rx1Data == '\r')
	{
		str2[rx1Head] = '\0';
		rx1Head = 0;
		rx1Flag = 1;
	}
	else
	{
		str2[rx1Head++] = rx1Data;
	}
}
