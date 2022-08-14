TARGET=shifter

LIB=../lib
SOURCES=$(wildcard *.c) $(wildcard $(LIB)/*.c)

PORT = /dev/ttyACM0
BAUD = 115200
DEVICE = atmega2560
PROGRAMMER = arduino
CLK=16000000UL

CC=avr-gcc
CFLAGS= -mmcu=$(DEVICE) -Os -DF_CPU=$(CLK)
INCLUDE = -I $(LIB)

default: print compile upload

print:
	@echo Sources: $(SOURCES)
	@echo Objects: $(OBJECTS)
	@echo Include: $(INCLUDE)

compile:
	$(CC) $(INCLUDE) ${CFLAGS} $(SOURCES) -o $(TARGET).o
	avr-objcopy -j .text -j .data -O ihex $(TARGET).o $(TARGET).hex

size:
	avr-size --format=avr --mcu=$(DEVICE) $(TARGET).o

upload:
	avrdude -C /etc/avrdude.conf -p $(DEVICE) -c wiring -D -P $(PORT) -b $(BAUD) -U flash:w:$(TARGET).hex
