#include "main.h"
uint16_t y_value = 0;

// ADC 초기화
void ADC_init()
{
	ADMUX = (1 << REFS0);  // 기준 전압 => 5V로 설정
	
	// ADEN                => ADC enable
	// ADPS2, ADPS1, ADPS0 => 128 분주비
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);  // 프리스케일러 128 (125kHz)
}

// ADC 값 읽기
uint16_t ADC_read(uint8_t channel)
{
	ADMUX = (ADMUX & 0xF8) | channel;  // 채널 선택
	ADCSRA |= (1 << ADSC);  // 변환 시작

	while (ADCSRA & (1 << ADSC));  // 변환 완료 대기
	return ADC;
}

void Timer_init()
{
	TCCR2 |= (1 << WGM21) | (1 << CS22); // CTC모드, 64분주
	TIMSK = (1 << OCIE2); // Timer2 comp 인터럽트 활성화 부분
	OCR2 = 249; // 0~249까지 카운트 되도록
	sei(); // 글로벌 인터럽트 활성화
}

// 타이머2 output compare 이용
// 64분주로 설정하게되면 주기 4us를 250번 카운팅되면 => 1ms마다 인터럽트 발생됨
ISR(TIMER2_COMP_vect)
{
	y_value = ADC_read(1);  // y축 adc값 (0 ~ 1023)
	//uint16_t y_mapped = (y_value * 1000) / 1023; // 0 ~ 1000
	uint16_t y_mapped = y_value;
	UART1_printNum(y_mapped); // y값 (atmega -> 블루투스)
	
	
	// pc에서 디버깅용
	UART0_print("y :");
	UART0_printNum(y_mapped);
}

// atmega -> 블루투스
void UART1_Transmit(unsigned char data)
{
	while(!(UCSR1A & (1 << UDRE1)));
	UDR1 = data;
}

void UART1_print(char *str)
{
	for(int i=0; str[i]; i++)
	{
		UART1_Transmit(str[i]);
	}
	//UART1_Transmit('\0'); // 문자열 보낼때
	UART1_Transmit('\n');
}

void UART1_printNum(uint16_t num)
{
	char buffer[10];
	itoa(num, buffer, 10);  // 정수 -> 문자열
	UART1_print(buffer);
}

// atmega -> pc
void UART0_Transmit(unsigned char data)
{
	// UDRE0는 송신버퍼가 비어있는지 확인하는것으로
	// 송신버퍼가 비어있으면 1, 아니면 0임
	while(!(UCSR0A & (1 << UDRE0)));
	UDR0 = data;
}

void UART0_print(char *str)
{
	// 배열을 받아서 배열의 크기만큼 반복문을 돌려라
	for(int i=0; str[i]; i++)
	{
		// 이때까지 받은 문자열을 송신
		UART0_Transmit(str[i]);
	}
	UART0_Transmit('\n');
}

void UART0_printNum(uint16_t num)
{
	char buffer[10];
	itoa(num, buffer, 10);  // 정수 -> 문자열
	UART0_print(buffer);
}


int main(void)
{
	UART0_Init();
	UART1_Init();
	DDRD |= 1 << PD3; // uart1의 tx핀
	DDRE |= 1 << PE1; // uart0의 tx핀
	
	ADC_init();
	Timer_init();
	sei();

	while (1)
	{
		
	}
}

