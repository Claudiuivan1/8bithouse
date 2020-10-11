#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "8bh_shell_commands.h"
#include "8bh_encoding.h"
#include "8bh_communication.h"
#include "../8bh_globals.h"

// Valid commands for dimmer
char buf_dim[3] = {13, '+', '-'};

// Welcome banner
const char *banner[]={
	"\n\t8BITHOUSE\n",
	"\tGeneralized client for 8bithouse\n",
	"\tusage:\n"
	"\t  $> 8bh <command> <parameters>\n",
	"\tcommands:",
	"\t-add <name_device>: add device on 8bithouse",
	"\t-rm <name_device> : remove device from 8bithouse",
	"\t-enable <name_device> : enable device on 8bithouse",
	"\t-disable <name_device>: disable device on 8bithouse",
	"\t-enable/disable buzzer : enable/disable the buzzer of the 8bithouse",
	"\t-reset : reset 8bithouse",
	"\t-list : list all the devices connected on the 8bithouse",
	"\t-dim <name_device> : change intensity of <name_device> on 8bithouse",
	"\t-exit : close client and communication with arduino\n",
	0
};

// Help Banner
const char *help[]={
	"\tCommands you can use are:",
	"\t\t- 8bh add <name_device>",
	"\t\t- 8bh rm <name_device>",
	"\t\t- 8bh enable/disable <name_device>/buzzer",
	"\t\t- 8bh reset",
	"\t\t- 8bh list",
	"\t\t- 8bh dim <name_device>",
	"\tIf you want to close the device, use:",
	"\t\t- exit",
	0
};

// Print banner
void printBanner(){
	const char*const* line = banner;
	while (*line) {
		printf("%s\n",*line);
		line++;
	}
}

// PRint helper
void printHelper(){
	const char* const* line = help;
	while (*line) {
		printf("%s\n",*line);
		line++;
	}
}

// SHELL FUNCTION FOR CLIENT
void shell_Start(void) {
	int ret, k;
	char print_buf[DIM_L], cmd[DIM_L];
	char* line = malloc(DIM_L*sizeof(char));
	char* buf_exit = malloc(DIM_L*sizeof(char));
	
	strcpy(buf_exit, "exit\n");
	
	// First open the serial, then start the communication with Arduino
	int fd = serial_open("/dev/ttyACM0");
    if (fd < 0) {
        printf("Error connecting to device\n");
		fflush(stdout);
		exit(0);
    }
    // Set serial
    int attrib_ok = serial_set_interface_attribs(fd, 115200, 0);
    serial_set_blocking(fd, 1);

	// Init buffers
	memset(print_buf, 0, DIM_L);
	memset(cmd, 0, DIM_L);	
	memset(line, 0, DIM_L);

	// Start communication with arduino
	initFn(fd, print_buf);
	fflush(stdout);

	printf("8bh> ");
	fgets(line,DIM_L,stdin);
	
	while(1){
		
		// If we have insert "exit", it close the client
		if (!strcmp(line,buf_exit)){
			
			printf("Closing device...\n");
			close(fd);
			fflush(stdout);
			break;
			
		} else{
			
			// Encoding of the entered command and sending of the generated string to Arduino
			ret = encode(line, cmd);
			if(ret){
				printf("Sending commands to device...\n");
				fflush(stdout);
				workFn(cmd, fd, print_buf);
			} else {
				printf("Command not found\n");
				fflush(stdout);
				printHelper();
			}
			
		}
	
		// If we have insert the DIM command, we send a single value at a time to Arduino  
		dimmer(ret, fd, buf_dim);

		memset(cmd, 0, DIM_L);
		memset(line, 0, DIM_L);
		
		printf("8bh> ");
		fgets(line, DIM_L, stdin);
		
	}
	
	// Memory free
	free(line);
	free(buf_exit);
	
}

int main(){
	
    // First it prints the welcome banner, then start the Shell
    printBanner();
    shell_Start();
    return 0;
    
}
