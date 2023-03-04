#include <stdlib.h>
#include <stdint.h>
#include "util/delay.h"
#include "avr/io.h"
#include "testing.h"

/* RAM test program
Tests in/out to every address
by writing and reading pseudorandom values
*/

// Wiring (with common GND)
// BUS  -> PROG ADDRL
// IN   -> A8
// OUT  -> A9
// ADDRL -> DATA A

// Remove -DBUS_FLIPPED in makefile for fixed programmer

// Time (us) to wait between clock edges
#define DELAY_TIME 1000
// true if data connected to B instead of A
#define DATA_B  false

#define IN_PIN 0
#define OUT_PIN 1

// Info for LSFR generator
#define LFSR_TAPS 0b10110100
#define LFSR_SEED 1
uint8_t lfsr;

int main(){
    // init bus with 2 outputs
    bus_init(0b11);
    // Set Address
    data_set(0, 0x7);
    for (uint8_t i=0; i<2; i++){
        _delay_ms(1000);
        io_set(OUT_PIN);
        _delay_ms(1000);
        io_clear(OUT_PIN);
    }


    // Main loop
    lfsr = LFSR_SEED;
    uint8_t lsb;
    for (uint16_t addr=0; addr<256; addr++){
        // set address
        data_set(0, addr & 0xFF);
        // Write value
        bus_write(IN_PIN, lfsr, DELAY_TIME);
        // check bus read
        if(bus_read(OUT_PIN, DELAY_TIME) != lfsr){
            while(1){};
        }
        // go to next value
        lsb = lfsr & 1;
        lfsr >>= 1;
        if (lsb){
            lfsr ^= LFSR_TAPS;
        }   
    }
    for (uint8_t i=0; i<2; i++){
        _delay_ms(1000);
        io_set(OUT_PIN);
        _delay_ms(1000);
        io_clear(OUT_PIN);
    }

}