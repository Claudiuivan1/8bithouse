#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include <stdlib.h>

#include "avr/eeprom.h"
#include "8bh_globals.h"
#include "8bh_eeprom.h"
#include "../8bh_globals.h"


void EEPROM_init(void){
    //initialize init, active and names array
    int size = 1;
    EEPROM_read((void*)&INIT_PRF, 0, size);
    EEPROM_read((void*)&ACTIVE_PRF, 1, size);
    EEPROM_read((void*)&BUZZER, 2, size);
    EEPROM_read((void*)&DIM_PRF,3,8);

    int i;
    int size_name = DIM_S;
    for (i = 0; i < 8; i++){
      char* new_device = (char*) malloc(DIM_S*sizeof(char));
      EEPROM_read(new_device, 11 + (size_name*i), size_name);
      NAMES_PRF[i] = new_device;  
    }
}

void EEPROM_read(void* dest_, const uint16_t src, uint16_t size){
  uint8_t * dest=(uint8_t*)dest_;
  uint16_t s=src;
  uint16_t s_end=src+size;
  while(s<s_end){
    eeprom_busy_wait();
    *dest=eeprom_read_byte((uint8_t*)s);
    ++s;
    ++dest;
  }
}

void EEPROM_write(uint16_t dest, const void* src_,  uint16_t size){
  const uint8_t * s=(uint8_t*)src_;
  const uint8_t * s_end=s+size;
  while(s<s_end){
    eeprom_busy_wait();
    eeprom_write_byte((uint8_t*)dest, *s);
    ++s;
    ++dest;
  }
}
