# AVR makefile stub

# to clean a folder
# from command line
#$> make clean

# to build the folder
#$> make

# to upload a file
# from command line
# $> make <main_filename>.hex

#
# remember to give yourself the read/write permissions on the
# serial line
# $> sudo addgroup <your username> dialout
# reboot



# !!!!!!!!!!!!!!!! MODIFY HERE !!!!!!!!!!!!!!!!

# put here the file containing the main routine
# to be uploaded on the avr
# you can add multiple files, they will be all generated

BINS=8bh_avr.elf

# put here the additional .o files you want to generate
# one .c file for each .o should be present
OBJS=8bh_encoding.c 8bh_globals.c 8bh_commands.c ../8bh_globals.c 8bh_eeprom.c 8bh_sound.c

# put here the additional header files needed for compilation
HEADERS=8bh_encoding.h 8bh_globals.h 8bh_commands.h ../8bh_globals.h 8bh_eeprom.h 8bh_sound.h buffer_utils.h

# the file below contains the actual rules

include ./avr.mk

# !!!!!!!!!!!!!!!! MODIFY HERE !!!!!!!!!!!!!!!!



