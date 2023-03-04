#include <stdlib.h>
#include <stdint.h>
#include "util/delay.h"
#include "avr/io.h"
#include "testing.h"

/* Bitwise test program for XOR gates
*/

// Wiring (with common GND)
// BUS    -> PROG ADDRL
// OUT      -> A9
// INPUT A -> DATA_A
// INPUT B -> DATA_B

// Remove -DBUS_FLIPPED in makefile for fixed programmer

// Pins
#define OUT_PIN 1


// Info for LSFR generator
#define LFSR_TAPS 0b10110100
#define LFSR_SEED 1


int main(){
    // init bus with 2 outputs
    bus_init(0b10);

    // Main loop
    uint8_t lfsr = LFSR_SEED;
    uint8_t lsb;
    uint8_t input_B = 0;
    for (uint16_t i=0; i<256; i++){
        for (uint16_t j=0; j<256; j++){
            // Set values to inputs A and B
            data_set(0, (uint8_t) i);
            data_set(1, (uint8_t) j);
            // Expected value is xor of inputs
            uint8_t target = i ^ j;
            
            if (bus_read(OUT_PIN, 10) != target){
                while(1){};
            }
        }
    }
    // clear output pin and set inputs to 0
    io_clear(OUT_PIN);
    data_set(0,0);
    data_set(1,0);


}