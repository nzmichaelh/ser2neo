APP ?= $(notdir $(PWD))

SRC = $(wildcard *.cc)
OBJ = $(SRC:%.cc=%.o)

CROSS_COMPILE = avr-
CC = $(CROSS_COMPILE)gcc
CXX = $(CROSS_COMPILE)g++
CFLAGS = -Os -DF_CPU=$(CLOCK) -mmcu=$(DEVICE) -Wall
CXXFLAGS = $(CFLAGS) -std=gnu++1y

all: $(APP).hex

usb-flash: $(APP).hex
	sudo $(HOME)/bin/micronucleus --run $<

%.elf: $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ)
	$(CROSS_COMPILE)size -d $@

%.hex: %.elf
	avr-objcopy -j .text -j .data -O ihex $< $@

clean:
	rm -f $(APP).hex $(APP).elf $(OBJ) *~ *.ii
