#include "eeprom.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "util/delay.h"

// Connect Display input to Address LOW
// Connect a common ground
// Iterate through, changing display mode and writing values
int main(void){
    // Initialise registers for RAM
    eeprom_setup();

    // Iterate through address space
    for (uint16_t address=1; address<256; address++){
        // Flip address due to EEPROM sockets being wrong way round
        uint16_t a_flip = (address << 1) & 0xAA;
        a_flip |= (address >> 1) & 0X55;
        eeprom_write(0, a_flip,0);
        _delay_ms(100);
    }
}