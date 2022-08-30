#include <stdlib.h>
#include <stdint.h>
#include "util/delay.h"
#include "avr/io.h"
#include "testing.h"

/* Register testing program
Tests Load, output, data and reset
by writing and reading pseudorandom values
*/

// Wiring (with common GND)
// REG BUS  -> PROG ADDRL
// REG IN   -> A8
// REG OUT  -> A9
// REG DATA -> PROG DATA_A

// Remove -DBUS_FLIPPED in makefile for fixed programmer

// Time (us) to wait between clock edges
#define DELAY_TIME 0
// true if data connected to B instead of A
#define DATA_B  false

// Info for LSFR generator
#define LFSR_TAPS 0b10110100
#define LFSR_SEED 1
uint8_t lfsr;

int main(){
    // init bus with 2 outputs
    bus_init(0b11);
    _delay_ms(1000);
    // Write alternating pattern to visual check
    bus_write(0, 0x55, 1000);
    _delay_ms(1000);

    // Main loop
    lfsr = LFSR_SEED;
    uint8_t lsb;
    while(1){
        // Write value
        bus_write(0, lfsr, DELAY_TIME);
        // check bus read
        if(bus_read(1, DELAY_TIME) != lfsr){
            while(1){};
        }
        // check data port read
        if(data_read(DATA_B) != lfsr){
            while(1){};
        }

        // go to next value
        lsb = lfsr & 1;
        lfsr >>= 1;
        if (lsb){
            lfsr ^= LFSR_TAPS;
        }
        // break if back to start
        if (lfsr == LFSR_SEED){
            break;
        }
        
    }

    // Reset register
    rst_set(1);
    rst_set(0);
    // check reset
    if (bus_read(1, DELAY_TIME) !=0){
        bus_set(0xFF);
    }
}