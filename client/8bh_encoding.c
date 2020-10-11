#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "8bh_encoding.h"
#include "8bh_shell_commands.h"

//Encoding the entered command
int encode(char* buf, char* ret){
	char* cmd;
	char* a = malloc(strlen(buf)+1);
	
    strcpy(a,buf);
    cmd = strtok(a, " ");
    
    if (!strcmp("8bh", cmd)){
        cmd = strtok(NULL, " ");

        if(cmd != NULL && cmd[strlen(cmd)-1] == '\n')
			cmd[strlen(cmd)-1] = 0;
			
        Command* command = findCommand(cmd);
        
        if (command){

            unsigned char c = command->cod ;
            unsigned char aux = (31<<3)|c;
            ret[0] = aux;
            
            cmd = strtok(NULL, " ");
            
            if (cmd == NULL){

                if (command->num_args == 0){
					
                    ret[1] = 0;
                    free(a);
                    return 1;
                    
                } else {
					
                    free(a);
                    ret = NULL;
                    return 0;
                    
                }
                
            } else {
				
				if(cmd[strlen(cmd)-1] == '\n')
					cmd[strlen(cmd)-1] = 0;
				 
                if (command->num_args == 0) {
					
                    printf("%s\n", command->help);
                    free(a);
                    ret = NULL;
                    return 0;
                    
                } else {
                    
                    ret[1] = (unsigned char)(strlen(cmd));
                    int i = 0;
                    for (i = 0; i < strlen(cmd); i++){
                        ret[2+i] = *(cmd+i);
                    }

                    cmd = strtok(NULL, " ");
                    
                    if (cmd != NULL){
                        free(a);
                        ret = NULL;
                        return 0;
                    }
                    
                    free(a);
                    ret[2+i] = 0;
                    if (ret[0] == -2) 
                        return 2;
                    return 1;
                }
            }
        } else {
            free(a);
            ret = NULL;
            return 0;
        }
    } else {
        free(a);
        ret = NULL;
        return 0;
    }
}

