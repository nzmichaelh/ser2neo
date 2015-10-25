#include "neopixel.h"
#include "serial.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>

extern Serial serial;

void NeoPixel::init() {
    DDRB |= _BV(1);
    TCCR0A = _BV(WGM01) | _BV(WGM00) | _BV(COM0B1) | _BV(COM0B0);
    TCCR0B = _BV(WGM02) | _BV(CS00);
    OCR0A = Reload;
    OCR0B = Low;
}

void NeoPixel::clear() {
    p_ = bits_;
    memset(bits_, Low, sizeof(bits_));
}

void NeoPixel::append(uint8_t r, uint8_t g, uint8_t b) {
    append(r);
    append(g);
    append(b);
}

void NeoPixel::append(uint8_t b) {
    if (p_ <= (bits_ + sizeof(bits_) - Tail - BitsPerColour)) {
        for (auto i = 0; i < BitsPerColour; i++) {
            *p_++ = (b & 0x80) ? High : Low;
            b <<= 1;
        }
    }
}

void NeoPixel::write() {
    bits_[sizeof(bits_) - 1] = Stop;

    uint8_t *p = bits_;
    uint8_t *pend = bits_ + sizeof(bits_);

    cli();

    OCR0B = Stop;
    TIFR |= _BV(OCF0A);
    while (!(TIFR & _BV(OCF0A))) {
    }
    TIFR |= _BV(OCF0A);
    while (!(TIFR & _BV(OCF0A))) {
    }
    OCR0B = *p++;

    for (; p != pend; p++) {
        while (!(TIFR & _BV(OCF0A))) {
        }
        OCR0B = *p;
        TIFR |= _BV(OCF0A);
    }
    OCR0B = Stop;

    sei();
}
