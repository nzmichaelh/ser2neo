APP = $(notdir $(PWD))
DEVICE     = attiny85
CLOCK      = 16500000
PROGRAMMER = -c signalyzer-lite
SRC = $(wildcard *.cc)
OBJ = $(SRC:%.cc=%.o)
FUSES = -U lfuse:w:0xe2:m -U hfuse:w:0xde:m -U efuse:w:0xfe:m

CROSS_COMPILE = avr-
CC = $(CROSS_COMPILE)gcc
CXX = $(CROSS_COMPILE)g++
CFLAGS = -Os -DF_CPU=$(CLOCK) -mmcu=$(DEVICE) -Wall
CXXFLAGS = $(CFLAGS) -std=gnu++11
AVRDUDE = avrdude $(PROGRAMMER) -p $(DEVICE)

all:	$(APP).hex

flash:	all
	$(AVRDUDE) -U flash:w:$(APP).hex:i

usb-flash: $(APP).hex
	sudo $(HOME)/bin/micronucleus --run $<

fuse:
	$(AVRDUDE) $(FUSES)

%.elf: $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ)
	$(CROSS_COMPILE)size -d $@

%.hex: %.elf
	avr-objcopy -j .text -j .data -O ihex $< $@

clean:
	rm -f $(APP).hex $(APP).elf $(OBJ) *~ *.ii
