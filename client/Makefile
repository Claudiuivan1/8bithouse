# Client makefile

CC=gcc
CFLAGS=-I.
DEPS = 8bh_communication.h 8bh_encoding.h 8bh_shell_commands.h ../8bh_globals.h
OBJ = 8bh.o 8bh_communication.o 8bh_encoding.o 8bh_shell_commands.o ./8bh_globals.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

8bh: 8bh.c 8bh_communication.c 8bh_encoding.c 8bh_shell_commands.c ../8bh_globals.c
	$(CC) -o $@ $^ $(CFLAGS)
