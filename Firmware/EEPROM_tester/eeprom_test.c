
#include "eeprom.h"
#include "util/delay.h"
#include "stdbool.h"

// EEPROM address size (13 or 15)
uint8_t ee_size = 15;
// EEPROM chip select
uint8_t ee_select = 0;

// Info for LSFR generator
#define LFSR_TAPS 0x6000
#define LFSR_SEED 1
uint16_t lfsr;
bool lsb;

void main(){
    // Init eeprom
    eeprom_setup();

    lfsr = LFSR_SEED;

    // Go through all posible values for input
    for (uint16_t input=0; input<(1<<ee_size); input++){

        while(!eeprom_write_verify(ee_select, input, lfsr & 0xFF)){
            prog_led(0);
            _delay_ms(500);
            prog_led(1);
            _delay_ms(500);
            while(1){};
        }
        // go to next value
        lsb = lfsr & 1;
        lfsr >>= 1;
        if (lsb){
            lfsr ^= LFSR_TAPS;
        }
        
    }
    // Finish write
    eeprom_finish();
}