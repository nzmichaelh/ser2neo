// Takes commands over serial and updates a NeoPixel LED ring.
//
// Copyright 2015 Google Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
#include <avr/interrupt.h>
#include <string.h>

#include "neopixel.h"
#include "serial.h"

/// Main application class.
///
/// Reads commands from the serial port, processes them, and
/// responds.
///
class Ser2Neo {
   public:
    void init();
    void run();

   private:
    static const uint8_t EOF = '\r';
    static const uint8_t ESC = '+';

    int read_escaped();
    void read_leds();
    void wait_eol();
    void send_ok(const char* msg=nullptr);

    NeoPixel leds;
    Serial serial;
};

/// Initialise the hardware.
void Ser2Neo::init() {
    leds.init();
    serial.init();
}

/// Read a character, de-escaping if required.
int Ser2Neo::read_escaped() {
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

/// Read the (potentially escaped) LED levels.
void Ser2Neo::read_leds() {
    leds.clear();
    while (true) {
        int got = read_escaped();
        if (got < 0) {
            return;
        }
        leds.append(got);
    }
}

/// Wait until the end-of-line character is received.
void Ser2Neo::wait_eol() {
    while (serial.getch() != EOF) {
    }
}

/// Send OK with an optional message.
void Ser2Neo::send_ok(const char* msg) {
    wait_eol();
    serial.putstr("OK");
    if (msg != nullptr) {
        serial.putstr(" ");
        serial.putstr(msg);
    }
    serial.putstr("\n");
}

/// Run the main loop.  Process and run commands.
void Ser2Neo::run() {
    sei();

    serial.putstr("READY\n");

    for (;;) {
        switch (serial.getch()) {
            case '?':
                send_ok("ser2neo 1 neopixel 16 grb");
                break;
            case '!':
                send_ok("sync");
                break;
            case 'l':
                read_leds();
                leds.write();
                serial.putstr("OK\n");
                break;
            case EOF:
                break;
            default:
                wait_eol();
                serial.putstr("ERROR\n");
                break;
        }
    }
}

static Ser2Neo ser2led;

int main() {
    ser2led.init();
    ser2led.run();

    return 0;
}
