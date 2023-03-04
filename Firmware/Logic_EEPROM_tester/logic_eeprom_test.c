#include <stdlib.h>
#include <stdint.h>
#include "util/delay.h"
#include "avr/io.h"
#include "testing.h"

/* Selector test program
Checks select and bus output
*/

// Wiring (with common GND)
// ADDRL    ->  ADDRL
// ADDRH    ->  ADDRH
// DATA     -> DATA_A
// SELECT   -> A8
// OUT      -> A9
// INPUT A -> DATA_A
// INPUT B -> DATA_B 

// Remove -DBUS_FLIPPED in makefile for fixed programmer

// Pins
#define SELECT_PIN 0
#define OUT_PIN 1

#define DATA_BUS 1

// Info for LSFR generator
#define LFSR_TAPS 0x6000
#define LFSR_SEED 0x4
uint16_t lfsr;
bool lsb;

int main(){
    // init bus
    bus_init(0);
    // Setup control pins
    ce0_set(0);
    ce1_set(1);
    oe_set(1);
    we_set(1);
    prog_set(1);
    _delay_ms(1000);

    lfsr = LFSR_SEED;

    for (uint16_t addr=(1 << 13); addr < (1<<14); addr++){
        // set the address
        uint8_t value = lfsr & 0xFF;
        addr_set(0, addr & 0xFF);
        addr_set(1,(uint8_t) (addr >> 8));
        // set the data
        data_set(DATA_BUS, value);
        _delay_us(1);
        // set write pin to low
        we_set(0);
        // wait then set high to write
        _delay_us(100);
        we_set(1);
        _delay_us(1);
        // read value
        data_clear(DATA_BUS);
        oe_set(0);
        _delay_us(100);
        if(data_read(DATA_BUS) != value){
            while(1);
        }
        oe_set(1);

        // go to next value
        lsb = lfsr & 1;
        lfsr >>= 1;
        if (lsb){
            lfsr ^= LFSR_TAPS;
        }
    }

    lfsr = LFSR_SEED;
    addr_set(0, 0);
    addr_set(1, 0);
    _delay_ms(1000);

    for (uint16_t addr=(1 << 13); addr < (1<<14); addr++){
        // set the address
        uint8_t value = lfsr & 0xFF;
        addr_set(0, addr & 0xFF);
        addr_set(1,(uint8_t) (addr >> 8));
        // set the data
        // read value
        data_clear(DATA_BUS);
        oe_set(0);
        _delay_us(100);
        if(data_read(DATA_BUS) != value){
            while(1);
        }
        oe_set(1);

        // go to next value
        lsb = lfsr & 1;
        lfsr >>= 1;
        if (lsb){
            lfsr ^= LFSR_TAPS;
        }
    }
    addr_set(0, 0xFF);
    addr_set(1, 0xFF);
    
    
}