#include <stdio.h>
#include <string.h>
#include <avr/io.h>

#include "./8bh_globals.h"

#define TCCRB_MASK ((1<<WGM12)|(1<<CS10))

// Init high registers
void init_compare() {
	OCR1BH = 255;
	OCR1AH = 255;
	OCR4CH = 255;
	OCR4BH = 255;
	OCR4AH = 255;
	OCR3AH = 255;
	OCR3BH = 255;
	OCR3CH = 255;
}

// Used to refresh output value of PINs
void refresh_output() {
	OCR1BL = 255 - (DIM_PRF[0] * (1&(ACTIVE_PRF>>7)));  
	OCR1AL = 255 - (DIM_PRF[1] * (1&(ACTIVE_PRF>>6)));
	OCR4CL = 255 - (DIM_PRF[2] * (1&(ACTIVE_PRF>>5)));
	OCR4BL = 255 - (DIM_PRF[3] * (1&(ACTIVE_PRF>>4)));
	OCR4AL = 255 - (DIM_PRF[4] * (1&(ACTIVE_PRF>>3)));
	OCR3AL = 255 - (DIM_PRF[5] * (1&(ACTIVE_PRF>>2)));
	OCR3BL = 255 - (DIM_PRF[6] * (1&(ACTIVE_PRF>>1)));
	OCR3CL = 255 - (DIM_PRF[7] * (1&(ACTIVE_PRF)));
}

// Used to refresh activated timers
void set_timers() {
	TCCR1A = (1<<WGM10)
			 |(1<<COM1A0)
			 |(1<<COM1A1)
			 |(1<<COM1B0)
			 |(1<<COM1B1);
	TCCR1B = TCCRB_MASK;

	TCCR3A = (1<<WGM10)
			 |(1<<COM3A0)
			 |(1<<COM3A1)
			 |(1<<COM3B0)
			 |(1<<COM3B1)
			 |(1<<COM3C0)
			 |(1<<COM3C1);
	TCCR3B = TCCRB_MASK;

	TCCR4A = (1<<WGM10)
			 |(1<<COM4A0)
			 |(1<<COM4A1)
			 |(1<<COM4B0)
			 |(1<<COM4B1)
			 |(1<<COM4C0)
			 |(1<<COM4C1);
	TCCR4B = TCCRB_MASK;
}
