typedef int (*CmdFn)(char*);
extern CmdFn commands_avr[];

CmdFn avr_findCommand(unsigned char name);
int avr_executeCommand(unsigned char name, char* arg);

int addFn(char* arg);
int rmFn(char* arg);
int enableFn(char* arg);
int disableFn(char* arg);
int resetFn(char* arg);
int listFn(char* arg);
int dimFn(char* arg);

struct UART* uart;

void UART_putString(char* s);
