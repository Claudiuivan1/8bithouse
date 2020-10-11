#include <stdio.h>

struct UART;

// Initializes a uart object
// Returns 0 on failure
struct UART*  UART_init(const char* device, uint32_t baud);

// Returns the size of the rx buffer
int UART_rxbufferSize(struct UART* uart);

// Returns the number of characters to be read fron the rx buffer
int UART_rxBufferFull(struct UART* uart);

// Returns the size of the tx buffer
int UART_txBufferSize(struct UART* uart);

// Returns the number of available characters in the tx buffer
int UART_txBufferFree(struct UART* uart);

// Returns the number of characters to be read fron the rx buffer
int UART_txBufferFull(struct UART* uart);

// Returns a character from the uart.
// If no character is available, the function waits
uint8_t UART_getChar(struct UART* uart);

// Puts a character in the buffer of a uart
// if the buffer is full, the function waits until
// there is room in the buffer
void UART_putChar(struct UART* uart, uint8_t c);

// Send string to client
void UART_putString(char* s);

// Decoding message from client
unsigned char decode_avr(char* buf, char* args);
