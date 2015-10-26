// NeoPixel ring driver.
//
#pragma once

#include <stdint.h>

// NeoPixel ring driver.
//
class NeoPixel {
   public:
    NeoPixel() { clear(); }

    void init();

    void clear();
    void append(uint8_t b);
    void write();

   private:
    static const int MaxLEDs = 16;

    static const int BitsPerColour = 8;
    static const int ColoursPerLED = 3;
    static const int Tail = 2;

    static const uint8_t Reload = F_CPU / 800000;
    static const uint8_t Low = Reload * 2 / 3;
    static const uint8_t High = Reload - Low;
    static const uint8_t Stop = 0xFF;

    uint8_t* p_;
    uint8_t bits_[MaxLEDs * ColoursPerLED * BitsPerColour + Tail];
};
