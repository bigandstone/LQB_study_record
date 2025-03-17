#ifndef __24c02_h__
#define __24c02_h__
    
#include "main.h"
#include <string.h>
#include <stdio.h>

#include "i2c_hal.h"

#define EPROM_ADDRESS_R 0xa1
#define EPROM_ADDRESS_W 0xa0

void eeprom_write(uint8_t address, uint16_t num, uint8_t* pdata);
void eeprom_read(uint8_t address, uint16_t num, uint8_t* pdata);

uint8_t eeprom_read_byte(uint8_t address);
void eeprom_write_byte(uint8_t address, uint8_t data);

//not allow skip page
void eeprom_write_page(uint8_t address, uint16_t num, uint8_t* pdata,uint8_t begin_index);

#endif
