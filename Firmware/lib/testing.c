#include "testing.h"
#include "avr/io.h"
#define __DELAY_BACKWARD_COMPATIBLE__
#include "util/delay.h"

struct bus bus;

struct address address;

void bus_init(uint8_t io){
    // Set CLK and RST pins as outputs
    #ifdef BUS_FLIPPED
    bus.clk_port = &PORTB;
    bus.clk_ddr = &DDRB;
    bus.rst_port = &PORTH;
    bus.rst_ddr = &DDRH;
    bus.clk_pin = 7;
    bus.rst_pin = 6;
    address.a15_ddr = &DDRB;
    address.a15_port = &PORTB;
    address.a15_pin = 6;
    address.oe_ddr = &DDRK;
    address.oe_port = &PORTK;
    address.oe_pin = 6;
    address.ce0_ddr = &DDRB;
    address.ce0_port = &PORTB;
    address.ce0_pin = 4;
    address.ce1_ddr = &DDRB;
    address.ce1_port = &PORTB;
    address.ce1_pin = 5;
    #else
    bus.clk_port = &PORTH;
    bus.clk_ddr = &DDRH;
    bus.rst_port = &PORTB;
    bus.rst_ddr = &DDRB;
    bus.clk_pin = 6;
    bus.rst_pin = 7;
    address.a15_ddr = &DDRK;
    address.a15_port = &PORTK;
    address.a15_pin = 6;
    address.oe_ddr = &DDRB;
    address.oe_port = &PORTB;
    address.oe_pin = 6;
    address.ce0_ddr = &DDRB;
    address.ce0_port = &PORTB;
    address.ce0_pin = 5;
    address.ce1_ddr = &DDRB;
    address.ce1_port = &PORTB;
    address.ce1_pin = 4;
    #endif
    clk_set(0);
    rst_set(0);

    // Enable outputs for control lines
    DDRK = io;

}

void clk_set(bool state){
    *bus.clk_ddr |= (1 << bus.clk_pin);
    if (state){
        *bus.clk_port |= (1 << bus.clk_pin);
    } else {
        *bus.clk_port &= ~(1 << bus.clk_pin);
    }
    
}

void rst_set(bool state){
    *bus.rst_ddr |= (1 << bus.rst_pin);
    if (state){
        *bus.rst_port |= (1 << bus.rst_pin);
    } else {
        *bus.rst_port &= ~(1 << bus.rst_pin);
    }
    
}

void prog_set(bool state){
    clk_set(state);
}

void we_set(bool state){
    rst_set(state);
}

void oe_set(bool state){
    *address.oe_ddr |= (1 << address.oe_pin);
    if (state){
        *address.oe_port |= (1 << address.oe_pin);
    } else {
        *address.oe_port &= ~(1 << address.oe_pin);
    }
}

void ce0_set(bool state){
    *address.ce0_ddr |= (1 << address.ce0_pin);
    if (state){
        *address.ce0_port |= (1 << address.ce0_pin);
    } else {
        *address.ce0_port &= ~(1 << address.ce0_pin);
    }
}

void ce1_set(bool state){
    *address.ce1_ddr |= (1 << address.ce1_pin);
    if (state){
        *address.ce1_port |= (1 << address.ce1_pin);
    } else {
        *address.ce1_port &= ~(1 << address.ce1_pin);
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

uint8_t bus_read(uint8_t pin, uint32_t delay){
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

void bus_write(uint8_t pin, uint8_t value, uint32_t delay){
    // set input pin high
    clk_set(0);
    io_set(pin);
    bus_set(value);
    // set clk low
    
    _delay_us(delay);
    // Rising edge on clock
    clk_set(1);
    _delay_us(delay);
    io_clear(pin);
    clk_set(0);
    

    bus_clear();
    // reset input pin
    
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

// Set either low address bytes or upper 7 bits
void addr_set(bool high, uint8_t value){
    if (!high){
        bus_set(value);
    } else {
        // Check 7th bit
        bool a15 = (value >> 6) & 1;
        #ifdef BUS_FLIPPED
        value = ((value << 1) & 0xAA) + ((value >> 1) & 0x55);
        #endif
        // Cannot set 7th bit if flipped, so set after
        DDRK |= 0x3F;
        PORTK &= (0xC0);
        PORTK |= value;

        *address.a15_ddr |= (1 << address.a15_pin);
        if (a15){            
            *address.a15_port |= (1 << address.a15_pin);
        } else {
            *address.a15_port &= ~(1 << address.a15_pin);
        }
        
    }
}
