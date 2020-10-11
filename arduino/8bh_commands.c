#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/io.h>

#include "8bh_globals.h"
#include "../8bh_globals.h"
#include "8bh_commands.h"
#include "8bh_encoding.h"
#include "8bh_eeprom.h"
#include "8bh_sound.h"

struct UART* uart;

// Array for Arduino subroutines
CmdFn commands_avr[] = {
	addFn,
    rmFn,
    enableFn,
    disableFn,
    resetFn,
    listFn,
    dimFn
};


// Used to verify if command exists
// If something is found the function pointer is retourned
CmdFn avr_findCommand(unsigned char name){
    unsigned char n = (name & 7);
    if(n < (unsigned char)(sizeof(commands_avr)/sizeof(CmdFn))){
        return commands_avr[n];
    }
    return 0;
}

// Used to execute specific command
// Special code is assigned for any command
int avr_executeCommand(unsigned char name, char* arg){
	int ret;
    unsigned char n = (name & 7);
    
    if(arg != NULL)
        ret = (*(commands_avr[n]))(arg);
    else
        ret = (*(commands_avr[n]))(NULL);
    
    //write variables on the eeprom
    //0    	1     	INIT_PRF
    //1 	2     	ACTIVE_PRF  
    //2 	514     NAMES_PRF
    int size = 1;
    EEPROM_write(0, (const void*)&INIT_PRF, size);
    EEPROM_write(1, (const void*)&ACTIVE_PRF, size);
    EEPROM_write(2, (const void*)&BUZZER, size);
    EEPROM_write(3, (const void*)&DIM_PRF, 8);
    int i;
    int size_name = 64;
    for (i = 0; i < 8; i++)
      EEPROM_write(11 + (size_name*i), (const void*)NAMES_PRF[i], size_name);

    return ret;
}

// **** ADD **** //
// Initialize new device
// Puts 1 in the first free position of INIT_PRF
// Puts the name of device, passed as argument in NAMES_PRF
int addFn(char* arg){
    int i;
    for(i = 0; ((INIT_PRF>>(7-i))&1) && i < 8; i++){
        if (!strcmp(arg, NAMES_PRF[i])){
            UART_putString("Device not added. Device already exists\r"); 
            return 0;
        }
    }
    if(i > 7) {
        UART_putString("Device not added. The space is full. You need to remove a device first\r");
        return 0;
    } else {
            SET(&INIT_PRF, i, 1);
            char* new_device = malloc(DIM_S*sizeof(char));
            strcpy(new_device, arg);
            NAMES_PRF[i] = new_device;
            if(BUZZER) ok_sound();
            UART_putString("Device added\r");
            return 1;
    }
}

// **** RM **** //
// Search device in NAMES_PRF
// Puts 0 in the respective position of INIT_PRF
// Puts a free char buffer in the respective position of NAMES_PRF
int rmFn(char* arg){
    int i;
    for(i = 0; i < 8; i++) {
        if(strcmp(NAMES_PRF[i], arg) == 0) {
            SET(&INIT_PRF, i, 0);
            SET(&ACTIVE_PRF, i, 0);
            DIM_PRF[i] = 255;
            free(NAMES_PRF[i]);
            NAMES_PRF[i] = NULL;
            if(BUZZER) ok_sound();
            UART_putString("Device Removed\r");
            return 1;
        }
    }
    UART_putString("Device not removed. Device not found. Maybe the name was wrong\r");
    return 0;
}

// **** ENABLE **** //
// Search device in NAMES_PRF
// Puts 1 in the respective position of ACTIVE_PRF
int enableFn(char* arg){
    int i;
    for(i = 0; i < 8; i++) {
        if (strcmp("buzzer", arg) == 0){
            BUZZER = 1;
            refresh_output();
            if(BUZZER) ok_sound();
            UART_putString("Buzzer enabled\r");
            return 1;
        }
        else if(strcmp(NAMES_PRF[i], arg) == 0) {
            SET(&ACTIVE_PRF, i, 1);
            DIM_PRF[i] = 135;
            refresh_output();
            if(BUZZER) ok_sound();
            UART_putString("Device enabled\r");
            return 1;
        }
    }
    UART_putString("Device not enabled. Device not found. Maybe the name was wrong\r");
    return 0;
}

// **** DISABLE **** //
// Search device in NAMES_PRF
// Puts 0 in the respective position of ACTIVE_PRF
int disableFn(char* arg){
    int i;
    for(i = 0; i < 8; i++) {
        if (strcmp("buzzer", arg) == 0){
            BUZZER = 0;
            refresh_output();
            UART_putString("Buzzer disabled\r");
            return 1;
        }
        else if(strcmp(NAMES_PRF[i], arg) == 0) {
            SET(&ACTIVE_PRF, i, 0);
            DIM_PRF[i] = 255;
            refresh_output();
            if(BUZZER) ok_sound();
            UART_putString("Device disabled\r");
            return 1;
        }
    }
    UART_putString("Device not disabled. Device not found. Maybe the name was wrong\r");
    return 0;
}

// **** RESET **** //
// Resets all the data structs
// INIT_PRF and ACTIVE_PRF are set to 0
// NAMES_PRF is emptied
//buzzer is enable by default
int resetFn(char* arg){
	int i;
	BUZZER = 1;
    INIT_PRF = 0;
    ACTIVE_PRF = 0;
    for (i=0;i<8;i++) {
		free(NAMES_PRF[i]);
        NAMES_PRF[i] = NULL;
        DIM_PRF[i] = 255;
    }
    refresh_output();
    if(BUZZER) ok_sound();    
    UART_putString("8bh reset done\r");
    return 1;
}

// **** LIST **** //
// Show all the devices added, enabled/disabled and brightness
int listFn(char* arg){
	int i, n_dev = 0;
	
	char ret[1024];
	memset(ret, 0, 1024);
	
	sprintf(ret + strlen(ret), "\n\n");
	
	for(i = 0; i < 8; i++)
		if((INIT_PRF>>(7-i))&1) 
			n_dev++;
			
	sprintf(ret + strlen(ret), "%d ASSOCIATED DEVICES\n\n", n_dev);
	
	for(i = 0; i < 8; i++)
		if((INIT_PRF>>(7-i))&1 && ACTIVE_PRF>>(7-i)&1) 
			sprintf(ret + strlen(ret), "%d. - %s (%d - %d%%)\n", i+1, NAMES_PRF[i], ACTIVE_PRF>>(7-i)&1, (DIM_PRF[i]*100/255));
		else if((INIT_PRF>>(7-i))&1 && !(ACTIVE_PRF>>(7-i)&1))
			sprintf(ret + strlen(ret), "%d. - %s (%d)\n", i+1, NAMES_PRF[i], ACTIVE_PRF>>(7-i)&1);
		else
			sprintf(ret + strlen(ret), "%d. - EMPTY\n", i+1);
		
			
	sprintf(ret + strlen(ret), "\r");
	
	if(BUZZER) ok_sound();
	
    UART_putString(ret);
    return 1;
}

// **** DIMMER ****//
// Modify the brightness of the devices
int dimFn(char* arg){
    int i;
    unsigned char cmd;
    
    for(i = 0; i < 8; i++) {
        if(strcmp(NAMES_PRF[i], arg) == 0 && ((ACTIVE_PRF>>(7-i))&1) == 1) {
			UART_putString("Reglate brightness:\n\n+ -> Increase\n- -> Decrease\nENTER -> Done\n\r");
			UART_putChar(uart, (char)1);
			
            while((cmd = UART_getChar(uart)) != 13) {
                if(cmd == 45 && DIM_PRF[i]>=30) 	{	// +
					
                    DIM_PRF[i]-=15;
                    refresh_output();
            
				} else if(cmd == 43 && DIM_PRF[i]<=240) { 	// -
					
                    DIM_PRF[i]+=15; 
                    refresh_output();
                    
                }	   
            }
            
			if(BUZZER) ok_sound();
            return 1;
        } else if(strcmp(NAMES_PRF[i], arg) == 0 && ((ACTIVE_PRF>>(7-i))&1) == 0){
			UART_putString("Device not enabled. Maybe you want to enable it first?\r");
			UART_putChar(uart, (char)0);
			return 0;
		}
    }
    UART_putString("Device not found. Maybe the name was wrong\r");
    UART_putChar(uart, (char)0);
    return 0;
}
