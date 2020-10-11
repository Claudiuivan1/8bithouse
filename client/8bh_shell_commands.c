#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "8bh_encoding.h"
#include "8bh_shell_commands.h"
#include "8bh_communication.h"
#include "../8bh_globals.h"

// Commands for 8bh
Command commands[]={
	{
        .name = "add",
        .num_args = 1,
        .fn = workFn,
        .cod = 0,
        .help = "usage: 8bh add <name_per>"

    },
    {
        .name = "rm",
        .num_args = 1,
        .fn = workFn,
        .cod = 1,
        .help = "usage: 8bh rm <name_per>"
    },
    {
        .name = "enable",
        .num_args = 1,
        .fn = workFn,
        .cod = 2,
        .help = "usage: 8bh enable <name_per>"
    },
    {
        .name = "disable",
        .num_args = 1,
        .fn = workFn,
        .cod = 3,
        .help = "usage: 8bh disable <name_per>"
    },
    {
        .name = "reset",
        .num_args = 0,
        .fn = workFn,
        .cod = 4,
        .help = "usage: 8bh reset"
    },
    {
        .name = "list",
        .num_args = 0,
        .fn = workFn,
        .cod = 5,
        .help = "usage: 8bh list"
    },
    {
        .name = "dim",
        .num_args = 1,
        .fn = workFn,
        .cod = 6,
        .help = "usage: 8bh dim <name_per>"
    }
};

const int num_commands = sizeof(commands)/sizeof(Command);

// Function that looks for the command between the commands of 8bh
Command* findCommand(const char* name){
	int cmd_idx = 0;
	while(cmd_idx < num_commands){
		if (!strcmp(commands[cmd_idx].name, name))
			return &commands[cmd_idx];
		cmd_idx++;
	}
	return 0;
}


// Function that manages the communication between client and arduino
int workFn(char* cmd, int fd, char* buf){
    int k, res, i = 0, j = 0;
    char c;
    
    while(write(fd, &cmd[i], 1)) {
        if(cmd[i] == '\r' || cmd[i] == '\0') break;
        i++;
    }
    
    printf("Waiting for reply...\n");
	fflush(stdout);
    
    while(read(fd, &c, 1) > 0) {
        if(c == '\r' || c == '\0') break;
        buf[j] = c;
        j++;
    }
    
    printf("ARDUINO: %s\n", buf);
    fflush(stdout);
    
    memset(buf, 0, DIM_L);
    memset(cmd, 0, DIM_L);
}

// Function that inits the communication
int initFn(int fd, char* buf){
	int k, i = 0;
	char c;
    
    memset(buf, 0, DIM_L);
        
    while(read(fd, &c, 1)) {
        if(c == '\r' || c == '\0') break;
        buf[i] = c;
        i++;
    }
    
    printf("Connecting to device...\n");
    
    printf("ARDUINO: %s\n", buf);
    fflush(stdout);

    memset(buf, 0, DIM_L);
}

// Function that communicates with arduino to dim device
void dimmer(int ret, int fd, char* buf_dim){
    if (ret == 2){
		char cmd_dim;
        char avr_ret[2];
        int exit_dim = 0;
            
        read(fd, &avr_ret, 1);
            
        if(avr_ret[0]) {
            
            while (1){
                system("/bin/stty raw");
                cmd_dim = getchar();
                system("/bin/stty cooked");
                printf("\n");
                
                for (int i = 0; i<sizeof(buf_dim);i++){
					if(cmd_dim == buf_dim[i]){
						write(fd, &cmd_dim, 1);
						if(cmd_dim == 13)
							exit_dim = 1;
						break;
					}
                }
                
                if (exit_dim)
					break;
            }
        }
    }
}
