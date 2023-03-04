#include "eeprom.h"
#include <stdlib.h>
#include <string.h>
#include "util/delay.h"

// 7 segment display positions (a=0 etc)
//   a
// f   b
//   g
// e   c
//   d
//       h

// Hex digits 0 to F
uint8_t digits[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6F,0x77,0x7c,0x39,0x5e,0x79,0x71};

// Invert for common cathode displays (or anode i'm not sure)
uint8_t invert = 0xFF;

// Chip select
uint8_t ce = 0;

// Number of modes
uint8_t nmodes = 4;

// Calulates the output for each case
uint8_t calc_output(uint8_t mode, uint8_t digit, uint8_t value){
    // init output value
    uint8_t output = 0;

    // Switch between each mode
    switch(mode){
        // UNSIGNED INT
        case 0:
        switch(digit){
            case 0:
            break;
            case 1:
            output = digits[value/100];
            break;
            case 2:
            output = digits[(value/10)%10];
            break;
            case 3:
            output = digits[value%10];
            break;
        }
        break;

        // SIGNED INT
        case 1:
        ;
        int8_t svalue;
        memcpy(&svalue, &value, 1);
        switch(digit){
            case 0:
            if (svalue < 0){
                output = 0x40;
            }
            break;
            case 1:
            output = digits[abs(svalue)/100];
            break;
            case 2:
            output = digits[(abs(svalue)/10)%10];
            break;
            case 3:
            output = digits[abs(svalue)%10];
            break;
        }
        break;

        // HEX
        case 2:
        switch(digit){
            case 0:
            output = digits[0];
            break;
            case 1:
            output = 0x74;
            break;
            case 2:
            output = digits[value >> 4];
            break;
            case 3:
            output = digits[value & 0x0F];
            break;
        }
        break;

        // Binary
        case 3:
        switch(digit){
            case 0:
            if((value >> 7) & 1){
                output |= 1<<4;
            }
            if((value >> 6) & 1){
                output |= 1<<2;
            }
            break;
            case 1:
            if((value >> 5) & 1){
                output |= 1<<4;
            }
            if((value >> 4) & 1){
                output |= 1<<2;
            }
            break;
            case 2:
            if((value >> 3) & 1){
                output |= 1<<4;
            }
            if((value >> 2) & 1){
                output |= 1<<2;
            }
            break;
            case 3:
            if((value >> 1) & 1){
                output |= 1<<4;
            }
            if((value >> 0) & 1){
                output |= 1<<2;
            }
            break;
        }
    }
    return output;
}

// Goes through each mode,digit and value and
// writes and verifies the EEPROM
int main(void){
    // Initialise registers for EEPROM
    eeprom_setup();

    // Number of digits
    uint8_t ndigits = 4;
    // input value and address
    uint8_t value = 0;
    uint16_t addr;

    // Iterate through display modes and each digit of display
    for (uint8_t mode=0; mode<nmodes; mode++){
        for(uint8_t digit=0; digit<ndigits; digit++){
            // reset value
            value = 0;
            
            // iterate through 256 possible values
            // while loop because for loop overflows and runs indefinitely
            while(1){
                // Set addr 
                addr = ((digit | (mode << 2)) << 8) + value;

                // get output value
                uint8_t output = calc_output(mode, digit, value);                

                // Write to EEPROM
                while (!eeprom_write_verify(ce, addr, output^invert)){
                    prog_led(0);
                    _delay_ms(500);
                    prog_led(1);
                    _delay_ms(500);
                }
                value++;
                if (!value){
                    break;
                }
            }
        }
    }
    eeprom_finish();
}