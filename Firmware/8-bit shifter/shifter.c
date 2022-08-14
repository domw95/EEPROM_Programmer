#include "shifter.h"
#include "eeprom.h"
#include "util/delay.h"
#include "stdbool.h"

// EEPROM address size (13 or 15)
uint8_t ee_size = 15;
// EEPROM chip select
uint8_t ee_select = 0;

void main(){
    // Init eeprom
    eeprom_setup();

    // Init variables
    uint8_t value;
    uint8_t instruction;
    uint8_t arg;
    uint8_t output;
    bool write;

    // Go through all posible values for input
    for (uint16_t input=0; input<(1<<ee_size); input++){
        value = input & 0xff;
        instruction = input >> 11;
        arg = (input >> 8) & 0b111;
        output = 0;
        write = true;

        switch(instruction){
            // Shift right
            case 0:
            output = value >> arg;
            break;

            // Shift left
            case 1:
            output = value << arg;
            break;

            // Bit Set
            case 2:
            output = value |= (1 << arg);
            break;

            // Bit clear
            case 3:
            output = value &= !(1 << arg);
            break;

            // Rotate right
            case 4:
            write = false;
            break;

            // Rotate left
            case 5:
            write = false;
            break;

            // Don't write if output noot set
            default:
            write = false;

        }

        // Write if output set
        if (write){
            // Keep trying to write 
            while(!eeprom_write_verify(ee_select,input,output)){
                prog_led(0);
                _delay_ms(50);
                prog_led(1);
                _delay_ms(50);
            }
        }
        
    }
    // Finish write
    eeprom_finish();
}