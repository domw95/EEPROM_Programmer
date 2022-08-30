#include "eeprom.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "util/delay.h"

// RAM has 13 bit address (8192 bytes)
// Address 13 is active high CE, so must be added to the address



// Iterate through, writing and verifying memory
int main(void){
    // Initialise registers for RAM
    eeprom_setup();

    // Iterate through address space
    for (uint16_t address=0; address<(1<<13); address++){

        if(!eeprom_write_verify(0,address+(1<<13), (uint8_t)address & 0xFF)){
            while(1){};   
        }
        if(!eeprom_write_verify(0,address+(1<<13), (uint8_t)(address ^ 0xFF))){
            while(1){};   
        }
        // _delay_ms(10);
    }
}