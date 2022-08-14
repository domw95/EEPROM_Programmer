#ifndef EEPROM_H
#define EEPROM_H

#include <stdint.h>
#include <stdbool.h>

// Initialises ports for communicating with EEPROM
void eeprom_setup();

// Disables LED and resets pin states
void eeprom_finish();

// Writes value to addr of eeprom specified by ce
void eeprom_write(uint8_t ce, uint16_t addr, uint8_t value);

// Reads value from addr of eeprom specified by ce
uint8_t eeprom_read(uint8_t ce, uint16_t addr);

// Writes and then reads value back. Quicker than seperate functions
bool eeprom_write_verify(uint8_t ce, uint16_t addr, uint8_t value);

// sets state of PROG LED
void prog_led(bool state);

#endif