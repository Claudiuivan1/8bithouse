#include <stdio.h>
#include <string.h>
#include <avr/io.h>

#define TCCRB_MASK ((1<<WGM12)|(1<<CS10))

unsigned char INIT_PRF;
unsigned char ACTIVE_PRF;
unsigned char DIM_PRF[8];
char* NAMES_PRF[8];
char BUZZER;

void init_compare(void); 
void refresh_output(void);
void set_timers(void);
