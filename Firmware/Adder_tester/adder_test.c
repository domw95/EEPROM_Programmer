#include <stdlib.h>
#include <stdint.h>
#include "util/delay.h"
#include "avr/io.h"
#include "testing.h"

/* Adder testing program
Tests Addition, subtraction, output and carry
*/

// Wiring (with common GND)
// BUS    -> PROG ADDRL
// SUB    -> A8
// OUT    -> A9
// CARRY  -> A10
// INPUT A -> DATA_A
// INPUT B -> DATA_B

// Remove -DBUS_FLIPPED in makefile for fixed programmer

// Pins (same order as on adder PCB)
#define SUB_PIN 0
#define OUT_PIN 1
#define CARRY_PIN 2

// Time (us) to wait between clock edges
#define DELAY_TIME 10000

// Info for LSFR generator
#define LFSR_TAPS 0b10110100
#define LFSR_SEED 1


int main(){
    // init bus with 2 outputs
    bus_init(0b11);

    // Main loop
    uint8_t lfsr = LFSR_SEED;
    uint8_t lsb;
    uint8_t input_B = 0;
    while(1){
        // Set inputs to adder
        data_set(0, lfsr);
        data_set(1, input_B);

        // clear subtract pin and wait
        io_clear(SUB_PIN);
        _delay_us(DELAY_TIME);

        // check output
        uint8_t target = lfsr + input_B;
        if (bus_read(OUT_PIN, DELAY_TIME) != target){
            while(1){};
        }

        // Check if carry should be set
        if (target<lfsr){
            if (!io_read(CARRY_PIN)){
                while(1){};
            }
        }

        // change to subtract and check value
        target = lfsr - input_B;
        io_set(SUB_PIN);
        _delay_us(DELAY_TIME);
        if (bus_read(OUT_PIN, DELAY_TIME) != target){
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
        input_B ++;
    }
    // clear subtract pin and set inputs to 0
    io_clear(SUB_PIN);
    data_set(0,0);
    data_set(1,0);


}