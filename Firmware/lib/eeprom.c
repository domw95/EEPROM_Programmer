#include "eeprom.h"
#include "avr/io.h"
#define F_CPU 16000000UL
#include <util/delay.h>

/*
    ### Wiring
    - Address LSB   - PORTF
    - Address MSB   - PORTK
    - Data          - PORTC
    - Spare Port    - PORTL
    - WE            - PORTB7    (13)
    - OE            - PORTB6    (12)
    - CE0           - PORTB5    (11)
    - CE1           - PORTB4    (10)
    - PROG          - PORTH6    (9)
*/
#define DELAY_TIME

uint8_t WE = 7;
uint8_t OE = 6;
uint8_t CE0 = 5;
uint8_t CE1 = 4;
uint8_t PROG = 6;

void eeprom_setup(){
    // Set address ports as outputs
    DDRF = 0xFF;
    DDRK = 0xFF;
    // Set control port as output
    DDRB = 0xF0;
    // Set PROG led pin as output and turn on
    DDRH = (1 << PROG);
    PORTH = (1 << PROG);

}

uint8_t eeprom_read(uint8_t ce, uint16_t addr){
    // Set address
    PORTF = addr & 0xFF;
    PORTK = addr >> 8;

    // Set data port as input
    DDRC = 0X00;
    PORTC = 0;

    // Enable chip with output enabled and read
    PORTB = (1 << WE) + (0 << OE) + (ce << CE0) + (!ce << CE1);
    _delay_ms(10);
    return PINC;
}

void eeprom_write(uint8_t ce, uint16_t addr, uint8_t value){
    // Set address
    PORTF = addr & 0xFF;
    PORTK = addr >> 8;

    // common portb values
    uint8_t portb = (1 << OE) + (ce << CE0) + (!ce << CE1);

    // Enable chip with output disabled
    PORTB = (1 << WE) + portb;

    // Set data port as an output and set value
    DDRC = 0xFF;
    PORTC = value;
    _delay_ms(1);

    // Enable write
    PORTB = (0 << WE) + portb;
    _delay_ms(1);

    // Finish Write
    PORTB = (1 << WE) + portb;

    // Set data as input
    DDRC = 0;
}

void prog_led(bool state){
    PORTH = (state << PROG);
}