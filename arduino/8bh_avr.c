#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "avr/eeprom.h"
#include "../8bh_globals.h"
#include "8bh_globals.h"
#include "8bh_encoding.h"
#include "8bh_commands.h"
#include "8bh_eeprom.h"
#include "8bh_sound.h"

// Init buffers and variables
char buf[DIM_L], args[DIM_S];
unsigned char cmd;
int ret, size;
uint8_t c;

int main(void){
	
	int size;
    
    // First initialization
    /* BUZZER 		-> ENABLED
     * INIT_PRF 	-> NO DEVICE REGISTERED
     * ACTIVE_PRF 	-> NO DEVICE ENABLED
     * NAMES_PRF 	-> NO NAME SAVED
     * DIM_PRF		-> NO INTENSITY REGISTERED
     */
     
    BUZZER = 1; 
	INIT_PRF = 0;
    ACTIVE_PRF = 0;
    for(int i = 0; i < 8; i++){
        NAMES_PRF[i] = NULL;
		DIM_PRF[i] = 255;
	}
		
	// Looking for saved values in EEPROM
    EEPROM_init();
    
    // Init communication through client and interrupt flag
    uart = UART_init("uart_0", 115200);

    sei();
    				
	// Init output ports
	
	DDRB |= 96;		//PORTA B : 7-8
	DDRH |= 56;		//PORTA H : 4-5-6
	DDRE |= 56;		//PORTA E : 1-2-3
	DDRA |= 1;		//PORTA A : BUZZER
	
	// Init timers and frequencies for output
	init_compare();
	set_timers();
	refresh_output();
	
	// Buzzer is on? Eventually execute init song
	execute_song(BUZZER);
    
    // Clean buffers
    memset(buf, 0, DIM_L);
    memset(args, 0, DIM_S);
    
	// Welcome client!
    UART_putString("Device connected. Communication can start\r");
    
    // Let's start working
    while(1) {
		
		// Get next command
		buf[0] = 0;
		size = 0;
		while(1) {
			
		  c = UART_getChar(uart);
		  buf[size] = c;
		  ++size;
		  buf[size] = 0;
		  if (c == 0xFF ||c == '\n' || c == '\r' || c == 0) {
			break;
		  }
		  
		}
		
		// Decoding and serving
		cmd = decode_avr(buf, args);
		ret = avr_executeCommand(cmd, args);	

		// Refreshing buffers and variables
		memset(buf, 0, DIM_L);
		memset(args, 0, DIM_S);
		cmd = 0;
		
    }
}
