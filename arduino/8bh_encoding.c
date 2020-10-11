#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/atomic.h>

#include "buffer_utils.h"
#include "8bh_commands.h"

#define UART_BUFFER_SIZE 256

// Defining UART communication struct
typedef struct UART {
	int tx_buffer[UART_BUFFER_SIZE];
	volatile uint8_t tx_start;
	volatile uint8_t tx_end;
	volatile uint8_t tx_size;

	int rx_buffer[UART_BUFFER_SIZE];
	volatile uint8_t rx_start;
	volatile uint8_t rx_end;
	volatile uint8_t rx_size;
  
	int baud;
	int uart_num; // hardware uart;
} UART;

static UART uart_0;

// Functions for baudrate setting
void setBaud57600(void) {
	#define BAUD 57600
	#include <util/setbaud.h>
		UBRR0H = UBRRH_VALUE;
		UBRR0L = UBRRL_VALUE;

	#ifdef USE_2X
		UCSR0A |= (1<<U2X0);
	#endif
	#undef BAUD
}

void setBaud115200(void) {
	#define BAUD 115200
	#include <util/setbaud.h>
		UBRR0H = UBRRH_VALUE;
		UBRR0L = UBRRL_VALUE;

	#ifdef USE_2X
		UCSR0A |= (1<<U2X0);
	#endif
	#undef BAUD
}

// Init communication function
struct UART* UART_init(const char* device __attribute__((unused)), uint32_t baud) {
	UART* uart = &uart_0;
	uart->uart_num = 0;

	switch(baud){
		case 57600: setBaud57600(); break;
		case 115200: setBaud115200(); break;
		default: return 0;
	}
  
	uart->tx_start = 0;
	uart->tx_end = 0;
	uart->tx_size = 0;
	uart->rx_start = 0;
	uart->rx_end = 0;
	uart->rx_size = 0;

	UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); // 8-bit data 
	UCSR0B = _BV(RXEN0) | _BV(TXEN0) | _BV(RXCIE0);   // Enable RX and TX 
	sei();  
	return &uart_0;
}

// Returns the free space in the buffer
int UART_rxbufferSize(struct UART* uart __attribute__((unused))) {
	return UART_BUFFER_SIZE;
}

// Number of chars in rx buffer
int UART_rxBufferFull(UART* uart) {
	return uart->rx_size;
}

// Returns the free occupied space in the buffer
int UART_txBufferSize(struct UART* uart __attribute__((unused))) {
	return UART_BUFFER_SIZE;
}

// Number of chars in rx buffer
int UART_txBufferFull(UART* uart) {
	return uart->tx_size;
}

// Number of free chars in tx buffer
int UART_txBufferFree(UART* uart){
	return UART_BUFFER_SIZE-uart->tx_size;
}

// Get single char
uint8_t UART_getChar(struct UART* uart){
	while(uart->rx_size==0);
	uint8_t c;
	ATOMIC_BLOCK(ATOMIC_FORCEON){
		c=uart->rx_buffer[uart->rx_start];
		BUFFER_GET(uart->rx, UART_BUFFER_SIZE);
	}
	return c;
}

// Put single char
void UART_putChar(struct UART* uart, uint8_t c) {
	while (uart->tx_size>=UART_BUFFER_SIZE);
	ATOMIC_BLOCK(ATOMIC_FORCEON){
		uart->tx_buffer[uart->tx_end]=c;
		BUFFER_PUT(uart->tx, UART_BUFFER_SIZE);
	}
	UCSR0B |= _BV(UDRIE0);
}

// Put string function
void UART_putString(char* s) {
	int l=strlen(s);
	for(int i=0; i<l; ++i, ++s)
		UART_putChar(uart, (uint8_t) *s);
}

// Decoding message from client to execute right command
unsigned char decode_avr(char* buf, char* args){
    unsigned char i, cmd, len;

    cmd = buf[0];
    len = buf[1];

    if(len)
        for(i = 0; i < len; i++)
            args[i] = buf[2+i];
    else
        args = NULL;

    return cmd;
}

//Interrupt service routines

ISR(USART0_RX_vect) {
  uint8_t c=UDR0;
  if (uart_0.rx_size<UART_BUFFER_SIZE){
    uart_0.rx_buffer[uart_0.rx_end] = c;
    BUFFER_PUT(uart_0.rx, UART_BUFFER_SIZE);
  }
}

ISR(USART0_UDRE_vect) {
  if (! uart_0.tx_size){
    UCSR0B &= ~_BV(UDRIE0);
  } else {
    UDR0 = uart_0.tx_buffer[uart_0.tx_start];
    BUFFER_GET(uart_0.tx, UART_BUFFER_SIZE);
  }
}
