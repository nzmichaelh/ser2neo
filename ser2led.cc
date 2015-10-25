#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <string.h>

#include "neopixel.h"
#include "serial.h"

extern Serial serial;

static NeoPixel leds;
Serial serial;

class Ser2Led {
   public:
    static void init();
    static void run();
};

void Ser2Led::init() {
    leds.init();
    serial.init();
}

uint8_t getch() { return 'x'; }

void send(const char* str) {
    for (; *str != '\0'; str++) {
        serial.putch((char)*str);
    }
}

static const uint8_t EOF = '\r';
static const uint8_t ESC = '+';

int read_escaped() {
    auto got = serial.getch();
    if (got == EOF) {
        return -1;
    }
    if (got == ESC) {
        got = serial.getch();
        if (got == EOF) {
            return -1;
        }
        got ^= ESC;
    }
    return got;
}

void read_leds() {
    leds.clear();
    for (;;) {
        int got = read_escaped();
        if (got < 0) {
            return;
        }
        leds.append(got);
    }
}

void wait_eol() {
    while (serial.getch() != EOF) {
    }
}

void Ser2Led::run() {
    sei();

    send("READY\n");

    for (;;) {
        switch (serial.getch()) {
            case '?':
                wait_eol();
                send("OK ser2led\n");
                break;
            case '!':
                wait_eol();
                send("OK sync\n");
                break;
            case 'w':
                wait_eol();
                leds.write();
                send("OK\n");
                break;
            case 'l':
                read_leds();
                leds.write();
                send("OK\n");
                break;
            case EOF:
                break;
            default:
                wait_eol();
                send("ERROR\n");
                break;
        }
    }
}

int main() {
    Ser2Led::init();
    Ser2Led::run();

    return 0;
}

#if 0
#if 0
    for (;;) {
        serial.putch('U');
        _delay_ms(2);
        serial.putch('x');
        _delay_ms(2);
    }
#endif
#if 1
    for (;;) {
        uint8_t sum = 0;
        for (;;) {
            auto got = serial.getch();
            if (got == '\r') {
                break;
            }
            sum += got;
        }
        serial.putch(sum);
    }
#endif
#if 1
    for (;;) {
        uint8_t got = serial.getch();
//        _delay_ms(2);
        serial.putch(got);
//        serial.putch(serial.getch());
    }
#endif
#if 1
    for (;;) {
        send("READY ");
//        send("R");
        _delay_ms(10);
    }
#endif
#if 0
    leds.clear();
    for (;;) {
        leds.write();
        _delay_ms(10);
    }
#endif
#endif
