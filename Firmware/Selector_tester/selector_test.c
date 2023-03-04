#include <stdlib.h>
#include <stdint.h>
#include "util/delay.h"
#include "avr/io.h"
#include "testing.h"

/* Selector test program
Checks select and bus output
*/

// Wiring (with common GND)
// BUS    -> PROG ADDRL
// SELECT   -> A8
// OUT      -> A9
// INPUT A -> DATA_A
// INPUT B -> DATA_B 

// Remove -DBUS_FLIPPED in makefile for fixed programmer

// Pins
#define SELECT_PIN 0
#define OUT_PIN 1

int main(){
    // init bus with 2 outputs
    bus_init(0b11);

    for (uint16_t i=0; i<256; i++){
        for (uint16_t j=0; j<256; j++){
            // Set values to inputs A and B
            data_set(0, (uint8_t) i);
            data_set(1, (uint8_t) j);
            // Select input A
            io_clear(SELECT_PIN);
            _delay_us(10);
            // Check value        
            if (bus_read(OUT_PIN, 10) != i){
                while(1){};
            }
            // Select input B
            io_set(SELECT_PIN);
            _delay_us(10);
            if (bus_read(OUT_PIN, 10) != j){
                while(1){};
            }
        }
    }
    // clear output pin and set inputs to 0
    io_clear(OUT_PIN);
    io_clear(SELECT_PIN);
    data_set(0,0);
    data_set(1,0);


}