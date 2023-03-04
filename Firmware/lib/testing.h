#ifndef TESTING_H
#define TESTING_H

#include <stdint.h>
#include <stdbool.h>


struct bus{
    volatile uint8_t* clk_port;
    volatile uint8_t* clk_ddr;
    uint8_t clk_pin;
    volatile uint8_t* rst_port;
    volatile uint8_t* rst_ddr;
    uint8_t rst_pin;
};

struct address {
    volatile uint8_t* a15_port;
    volatile uint8_t* a15_ddr;
    uint8_t a15_pin;
    volatile uint8_t* oe_port;
    volatile uint8_t* oe_ddr;
    uint8_t oe_pin;
    volatile uint8_t* ce0_port;
    volatile uint8_t* ce0_ddr;
    uint8_t ce0_pin;
    volatile uint8_t* ce1_port;
    volatile uint8_t* ce1_ddr;
    uint8_t ce1_pin;
};

extern struct bus bus;

extern struct address address;

void bus_init(uint8_t io);

void clk_set(bool state);

void rst_set(bool state);

void prog_set(bool state);

void we_set(bool state);

void oe_set(bool state);

void ce0_set(bool state);

void ce1_set(bool state);

void bus_set(uint8_t value);

void bus_clear();

uint8_t bus_read(uint8_t pin, uint32_t delay);

void bus_write(uint8_t pin, uint8_t value, uint32_t delay);

void data_set(bool B, uint8_t value);

void data_clear(bool B);

uint8_t data_read(bool B);

void io_set(uint8_t pin);

void io_clear(uint8_t pin);

bool io_read(uint8_t pin);

void addr_set(bool high, uint8_t value);

#endif
