#include "testing.h"
#include "avr/io.h"
#define __DELAY_BACKWARD_COMPATIBLE__
#include "util/delay.h"

struct bus bus;

void bus_init(uint8_t io){
    // Set CLK and RST pins as outputs
    #ifdef BUS_FLIPPED
    bus.clk_port = &PORTB;
    bus.clk_ddr = &DDRB;
    bus.rst_port = &PORTH;
    bus.rst_ddr = &DDRH;
    bus.clk_pin = 7;
    bus.rst_pin = 6;
    #else
    bus.clk_port = &PORTH;
    bus.clk_ddr = &DDRH;
    bus.rst_port = &PORTB;
    bus.rst_ddr = &DDRB;
    bus.clk_pin = 6;
    bus.rst_pin = 7;
    #endif
    *bus.clk_ddr = (1 << bus.clk_pin);
    *bus.rst_ddr = (1 << bus.rst_pin);

    // Enable outputs for control lines
    DDRK = io;

}

void clk_set(bool state){
    if (state){
        *bus.clk_port |= (1 << bus.clk_pin);
    } else {
        *bus.clk_port &= ~(1 << bus.clk_pin);
    }
    
}

void rst_set(bool state){
    if (state){
        *bus.rst_port |= (1 << bus.rst_pin);
    } else {
        *bus.rst_port &= ~(1 << bus.rst_pin);
    }
    
}

void bus_set(uint8_t value){
    DDRF = 0xFF;
    #ifdef BUS_FLIPPED
    value = ((value << 1) & 0xAA) + ((value >> 1) & 0x55);
    #endif
    PORTF = value;
}

void bus_clear(){
    DDRF = 0;
}

uint8_t bus_read(uint8_t pin, uint16_t delay){
    // set bus connector as input
    DDRF = 0;
    // set output pin high
    io_set(pin);
    // get value after delay
    _delay_us(delay);
    uint8_t value =  PINF;
    #ifdef BUS_FLIPPED
    value = ((value << 1) & 0xAA) + ((value >> 1) & 0x55);
    #endif
    // reset output pin
    io_clear(pin);
    return value;
}

void bus_write(uint8_t pin, uint8_t value, uint16_t delay){
    // set clk low
    clk_set(0);
    // set bus connector as output
    DDRF = 0xFF;
    // set input pin high
    io_set(pin);
    // set value
    #ifdef BUS_FLIPPED
    value = ((value << 1) & 0xAA) + ((value >> 1) & 0x55);
    #endif
    PORTF = value;
    _delay_us(delay);
    // Rising edge on clock
    clk_set(1);
    _delay_us(delay);
    clk_set(0);

    // set bus connector as input
    DDRF = 0x0;
    // reset input pin
    io_clear(pin);
}

void data_set(bool B, uint8_t value){
    #ifdef BUS_FLIPPED
    value = ((value << 1) & 0xAA) + ((value >> 1) & 0x55);
    #endif
    if (B){
        DDRL = 0xFF;
        PORTL = value;
    } else {
        DDRC = 0xFF;
        PORTC = value;
    }
}

void data_clear(bool B){
    if (B){
        DDRL = 0;
    } else {
        DDRC = 0;
    }
}

uint8_t data_read(bool B){
    uint8_t value;
    if (B){
        DDRL = 0;
        value = PINL;

    } else {
        DDRC = 0;
        value = PINC;
    }
    #ifdef BUS_FLIPPED
    value = ((value << 1) & 0xAA) + ((value >> 1) & 0x55);
    #endif
    return value;
}

inline void io_set(uint8_t pin){
    PORTK |= (1 << pin);
}

inline void io_clear(uint8_t pin){
    PORTK &= ~(1 << pin);
}

inline bool io_read(uint8_t pin){
    return (PINK >> pin) & 1;
}